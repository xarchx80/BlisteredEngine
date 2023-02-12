#pragma once

#define VS_ENTRY "VS"
#define PS_ENTRY "PS"
#define VS_VERSION "vs_5_0"
#define PS_VERSION "ps_5_0"

struct DX11Shader
{
	DX11Shader();
	~DX11Shader();

	void CreateFromCode(LPCSTR vs, LPCSTR ps);
	static void CompileCode(LPCSTR code, LPCSTR entry, LPCSTR version, ID3DBlob** blob);


	ID3D11VertexShader*		mVS;
	ID3D11PixelShader*		mPS;
	//ID3DBlob*				mByteBuffer;

	byte* GetVertexBinarybuffer() const { return (byte*)mVertexBinayBuffer.data(); }
	size_t GetVertexBinarySize() const { return mVertexBinayBuffer.size(); }

	void BindPipeline();
private:
	std::vector<byte>       mVertexBinayBuffer;
};