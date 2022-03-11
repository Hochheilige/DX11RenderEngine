#pragma once

#include <memory>
#include <tuple>

#include <WndDxIncludes.h>
#include <Window.h>
#include <Utils.h>

//#include <shaders/shader_defines.h>

// for now it will be a singletone with base dx11 structured reailized

//	cs_5_0	DirectCompute 5.0 (compute shader)
//	ds_5_0	Domain shader
//	gs_5_0	Geometry shader
//	hs_5_0	Hull shader
//	ps_5_0	Pixel shader
//	vs_5_0	Vertex shader

#define CS_SHADER_ENTRY 0b0000'0001
#define DS_SHADER_ENTRY 0b0000'0010
#define GS_SHADER_ENTRY 0b0000'0100
#define HS_SHADER_ENTRY 0b0000'1000
#define PS_SHADER_ENTRY 0b0001'0000
#define VS_SHADER_ENTRY 0b0010'0000

struct ShaderOptions
{
	D3D_SHADER_MACRO* mac;
	int entrys;
};

class Renderer {

private:
	Renderer();

private:
	Window*                  window;
	ID3D11Device*            device;
	ID3D11DeviceContext*     context;
	IDXGISwapChain*          swap_chain;
	ID3D11RenderTargetView*  render_target_view;
	ID3D11DepthStencilView*  depth_stencil_view;
	ID3D11DepthStencilState* depth_stencil_state;

	D3D11_VIEWPORT viewport;

	D3D_DRIVER_TYPE driver_type;

	ID3DUserDefinedAnnotation* perf;

	ID3D11SamplerState* sampler;

	bool is_initialized;

private:
	static Renderer* renderer;

public:
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;


	~Renderer();

	static Renderer* GetInstance();

	bool Initialize(const HINSTANCE instance, const WNDPROC wndproc);

	void BeginEvent(wchar_t* name);
	void EndEvent();
	void SetMarker(wchar_t* name);

public:

	void Clear();

	void Swap();

	ID3D11Device* GetDevice();

	ID3D11DeviceContext* GetContext();

	IDXGISwapChain* GetSwapChain();

	ID3D11RenderTargetView* GetRenderTargetView();

	std::tuple<float, float> GetWindowParameters();

	ID3D11DepthStencilView* GetDepthStencilView();

	HWND GetWindow();
};