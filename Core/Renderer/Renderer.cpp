#include "Renderer.h"


#pragma comment(lib,"d3d11.lib")
using std::make_unique;

Renderer* Renderer::renderer = nullptr;

Renderer::Renderer()
	: window(new Window()),
	device(nullptr),
	context(nullptr),
	swap_chain(nullptr),
	//render_target_view(),
	viewport({ 0 }),
	driver_type(D3D_DRIVER_TYPE_HARDWARE),
	sampler(nullptr) {
}

Renderer* Renderer::GetInstance() {
	if (renderer == nullptr) {
		renderer = new Renderer();
	}

	return renderer;
}

bool Renderer::Initialize(const HINSTANCE instance, const WNDPROC wndproc) {
	if (!is_initialized) {

		window->Initialize(instance, wndproc);

		UINT debug_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		debug_flags |= D3D11_CREATE_DEVICE_DEBUG;
#else
		debug_flags |= NULL;
#endif

		D3D_FEATURE_LEVEL feature_level;
		DXCHECK(
			D3D11CreateDevice(
				nullptr, driver_type, 0,
				debug_flags,
				nullptr, 0,
				D3D11_SDK_VERSION,
				&device,
				&feature_level,
				&context
			)
		);

		HRESULT hr = context->QueryInterface(__uuidof(perf), reinterpret_cast<void**>(&perf));
		if (FAILED(hr))
			return false;

		if (feature_level != D3D_FEATURE_LEVEL_11_0) {
			printf("[DX11]: Direct3D Feature Level 11 unsupported.");
			MessageBoxA(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
		}

		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

		const float width = window->GetWidth();
		const float height = window->GetHeight();

		swap_chain_desc.BufferCount = 2;
		swap_chain_desc.BufferDesc.Width = width;
		swap_chain_desc.BufferDesc.Height = height;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow = window->GetWindow();
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.Windowed = TRUE;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		IDXGIDevice* dxgi_device;
		IDXGIAdapter* adapter;
		IDXGIFactory* factory;
		DXCHECK(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device));
		DXCHECK(dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter));
		DXCHECK(adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory));
		DXCHECK(factory->CreateSwapChain(device, &swap_chain_desc, &swap_chain));

		dxgi_device->Release();
		adapter->Release();
		factory->Release();

		assert(device);
		assert(context);
		assert(swap_chain);

		ID3D11Texture2D* back_buffer;
		DXCHECK(swap_chain->GetBuffer(0u, __uuidof(ID3D11Texture2D), (void**)&back_buffer));
		DXCHECK(device->CreateRenderTargetView(back_buffer, 0, &render_target_view));

		back_buffer->Release();

		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
		ZeroMemory(&depth_stencil_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depth_stencil_desc.DepthEnable = true;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
		DXCHECK(device->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state));

		D3D11_TEXTURE2D_DESC depth_stencil_tex;
		ZeroMemory(&depth_stencil_tex, sizeof(D3D11_TEXTURE2D_DESC));
		depth_stencil_tex.Width = window->GetWidth();
		depth_stencil_tex.Height = window->GetHeight();
		depth_stencil_tex.MipLevels = 1;
		depth_stencil_tex.ArraySize = 1;
		depth_stencil_tex.SampleDesc.Count = 1;
		depth_stencil_tex.SampleDesc.Quality = 0;
		depth_stencil_tex.Format = DXGI_FORMAT_D32_FLOAT;
		depth_stencil_tex.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_tex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_tex.CPUAccessFlags = 0;
		depth_stencil_tex.MiscFlags = 0;

		ID3D11Texture2D* buffer;

		DXCHECK(device->CreateTexture2D(&depth_stencil_tex, 0, &buffer));

		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
		ZeroMemory(&depth_stencil_view_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depth_stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
		depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Texture2D.MipSlice = 0u;

		DXCHECK(device->CreateDepthStencilView(buffer, &depth_stencil_view_desc, &depth_stencil_view));

		buffer->Release();

		context->OMSetRenderTargets(1u, &render_target_view, depth_stencil_view);
		context->OMSetDepthStencilState(depth_stencil_state, 1);

		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<FLOAT>(window->GetWidth());
		viewport.Height = static_cast<FLOAT>(window->GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		context->RSSetViewports(1, &viewport);

		D3D11_SAMPLER_DESC sampler_desc;
		ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));

		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampler_desc.BorderColor[0] = 1.0f;
		sampler_desc.BorderColor[1] = 1.0f;
		sampler_desc.BorderColor[2] = 1.0f;
		sampler_desc.BorderColor[3] = 1.0f;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MinLOD = -FLT_MAX;
		sampler_desc.MaxLOD = FLT_MAX;

		device->CreateSamplerState(&sampler_desc, &sampler);

		context->PSSetSamplers(0, 1, &sampler);

		is_initialized = true;
		return true;
	}
	return false;
}

ID3D11Device* Renderer::GetDevice() {
	return device;
}

ID3D11DeviceContext* Renderer::GetContext() {
	return context;
}

IDXGISwapChain* Renderer::GetSwapChain() {
	return swap_chain;
}

ID3D11RenderTargetView* Renderer::GetRenderTargetView() {
	return render_target_view;
}

ID3D11DepthStencilView* Renderer::GetDepthStencilView() {
	return depth_stencil_view;
}

float Renderer::GetWindowWidth()
{
	return static_cast<float>(window->GetWidth());
}

float Renderer::GetWindowHeight()
{
	return static_cast<float>(window->GetHeight());
}

HWND Renderer::GetWindow()
{
	return window->GetWindow();
}

void Renderer::Clear() {
	context->ClearRenderTargetView(render_target_view, DirectX::Colors::GreenYellow);
	context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Renderer::Swap() {
	swap_chain->Present(1, 0);
}

Renderer::~Renderer()
{
	sampler->Release();
	render_target_view->Release();
	depth_stencil_state->Release();
	depth_stencil_view->Release();
	swap_chain->Release();
	context->Release();
	device->Release();
	window->~Window();
}

void Renderer::BeginEvent(wchar_t* name)
{
	perf->BeginEvent(name);
}

void Renderer::EndEvent()
{
	perf->EndEvent();
}

void Renderer::SetMarker(wchar_t* name)
{
	perf->SetMarker(name);
}