#include <Utils.h>

ID3DBlob* CompileShader(
	const wchar_t* filename,
	const D3D_SHADER_MACRO* defines,
	const char* entrypoint,
	const char* target)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	ID3DBlob* byteCode;
	ID3DBlob* errors;

	D3DCompileFromFile(filename, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint, target, compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
	{
		printf("L[DX11][ERROR]: %s", (char*)errors->GetBufferPointer());
		//con::Outf(L"[DX11][ERROR]:", (char*)errors->GetBufferPointer());
		//con::Out((char*)errors->GetBufferPointer());
	}

	return byteCode;
};