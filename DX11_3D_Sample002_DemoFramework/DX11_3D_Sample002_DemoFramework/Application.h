#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
/**
 * @brief Application Class
 */
class Application
{
public:
	
	//
	//! @brief �R���X�g���N�^
	//
	Application();

	
	//
	//! @brief �R���X�g���N�^
	//!
	//! @param [in] title �^�C�g����
	//
	Application(LPSTR title);

	//
	//! @brief �f�X�g���N�^
	//
	virtual ~Application();

	//
	//! @brief �A�v���P�[�V�������s
	//
	void Run();


	// -------------------------- Protected Fields ----------------------
protected:
	HINSTANCE m_hInst;					//!< �C���X�^���X�n���h��
	HWND m_hWnd;						//!< �E�B���h�E�n���h��
	D3D_DRIVER_TYPE m_DriverType;		//!< �h���C�o�[�^�C�v
	D3D_FEATURE_LEVEL m_FeatureLevel;	//!< �@�\���x��
	UINT m_MultiSampleCount;			//!< �}���`�T���v�����O�J�E���g��
	UINT m_MultiSampleQuality;			//!< �}���`�T���v���i��
	UINT m_SwapChainCount;				//!< �X���b�v�`�F�C���o�b�t�@��
	DXGI_FORMAT m_SwampChainFormat;		//!< �X���b�v�`�F�C���o�b�t�@�̃t�H�[�}�b�g
	DXGI_FORMAT m_DepthStencilFormat;	//!< �[�x�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	ID3D11Device* m_pDevice;				//!< �f�o�C�X
	ID3D11DeviceContext* m_pDeviceContext;	//!< �f�o�C�X�R���e�L�X�g
	IDXGISwapChain* m_pSwapChain;			//!< �X���b�v�`�F�C��
	RenderTarget2D m_RenderTarget2D;		//!< �`��^�[�Q�b�g
	DepthStencilBuffer m_DepthStencilBuffer;	//!< �[�x�X�e���V���o�b�t�@
	ID3D11RasterizerState* m_pRasterizerState;	//!< ���X�^���C�U�X�e�[�g
	ID3D11DepthStencilState* m_pDepthStencilState;	//!< �[�x�X�e���V���X�e�[�g
	ID3D11BlendState* m_pBlendState;			//!< �u�����h�X�e�[�g
	FLOAT m_BlendFactor[4];	//!< �u�����h�t�@�N�^�[
	UINT m_SampleMask;		//!< �T���v���}�X�N
	UINT m_StencilRef;		//!< �X�e���V���Q��
	FLOAT4 m_ClearColor;	//!< �w�i�̃N���A�J���[
	UINT m_Width;			//!< ��ʕ�
	UINT m_Height;			//!< ��ʍ���
	FLOAT m_AspectRatio;	//!< ��ʂ̃A�X�y�N�g��
	LPSTR m_Title;			//!< �^�C�g��
	Timer m_Timer;			//!< �^�C�}�[

	// -------------------------- Protected Methods ----------------------

	//
	//! @brief ����������
	//! 
	//! @note �h���N���X�ɂĎ������s��
	//
	virtual bool OnInit();
	
	//
	//! @brief �I��������
	//!
	//! @note �h���N���X�ɂĎ������s��
	//
	virtual void OnTerm();
	
	//
	//! @brief �t���[���J�ڎ��Ɏ��s���鏈��
	//!
	//! @param [in] time �A�v���P�[�V�����J�n����̑��Ύ���
	//! @param [in] elapsedTime �O�̃t���[������̌o�ߎ���
	//! @note �h���N���X�ɂĎ������s���B
	//
	virtual void OnFrameMove(double time, double elapsedTime);

	//
	//! @brief �t���[���`�掞�Ɏ��s���鏈��
	//!
	//! @param [in] time �A�v���P�[�V�����J�n����̑��Ύ���
	//! @param [in] elapsedTime �O�̃t���[������̌o�ߎ���
	//! @note �h���N���X�ɂĎ������s���B
	//
	virtual void OnFrameRender(double time, double elapsedTime);

	//
	//! @brief ���T�C�Y���Ɏ��s���鏈��
	//!
	//! @param [in] param ���T�C�Y�p�����[�^
	//! @note �h���N���X�ɂĎ������s���B
	//
	virtual void OnResize(const ResizeEventParam& param);

	//
	//! @brief �L�[�C�x���g�ʒm���Ɏ��s���鏈��
	//!
	//! @param [in] param �L�[�C�x���g�p�����[�^
	//! @note �h���N���X�ɂĎ������s���B
	//
	virtual void OnKey(const KeyEventParam& param);

	//
	//! @brief �}�E�X�C�x���g�ʒm���Ɏ��s���鏈��
	//!
	//! @param [in] param �}�E�X�C�x���g�p�����[�^
	//! @note �h���N���X�ɂĎ������s���B
	//
	virtual void OnMouse(const MouseEventParam& param);

	//
	//! @brief �`���~�t���O��ݒ�
	//!
	//! @param [in] isStopRendering �ݒ�l
	//
	void SetStopRendering(bool isStopRendering);

	//
	//! @brief �`���~�t���O���擾
	//!
	//! @retval true �`�揈�����Ăяo���Ȃ�
	//! @retval false �`�揈�����Ăяo��
	//
	bool IsStopRendering() const;

	//
	//! @brief �t���[���J�E���g���擾
	//!
	//! @return �t���[���J�E���g
	//
	DWORD GetFrameCount() const;

	//
	//! @brief FPS�擾
	//!
	//! @return FPS
	//
	FLOAT GetFPS() const;


	// -------------------------- Private Fields ----------------------

	bool m_IsStopRendering;		//!< �`���~�t���O
	DWORD m_FrameCount;			//!< �t���[���J�E���g
	FLOAT m_FPS;				//!< FPS
	DOUBLE m_LatestUpdateTime;	//!< �Ō�̍X�V����

	
	// -------------------------- Private Methods ----------------------

	//! @note �A�N�Z�X�֎~
	Application(const Application&); 

	//! @note �A�N�Z�X�֎~
	void operator =(const Application&);
	

	//
	//! @brief �A�v���P�[�V����������
	//!
	//! @return ������
	//! @note ������InitWnd()�AInitD3D()���Ăяo��
	//
	bool InitApp();

	//
	//! @brief �A�v���P�[�V�����I��
	//!
	//! @note ������TermWnd() TermD3D()���Ăяo��
	//
	void TermApp();

	//
	//! @brief �E�B���h�E������
	//!
	//! @return ������
	//
	bool InitWnd();

	//
	//! @brief �E�B���h�E�I��
	//
	void TermWnd();

	//
	//! @brief Direct3D������
	//
	bool InitD3D();

	//
	//! @brief Direct3D�I��
	//
	void TermD3D();

	//
	//! @breif ���C�����[�v����
	//
	void MainLoop();

	//
	//! @brief �L�[�C�x���g����
	//!
	//! @note �E�B���h�E�v���V�[�W������̌Ăяo����p�B
	//!       ������OnKey()���\�b�h���Ăяo���B
	//
	void KeyEvent(UINT nChar, bool isKeyDown, bool isAltDown);

	//
	//! @brief ���T�C�Y�C�x���g����
	//!
	//! @note �E�B���h�E�v���V�[�W������̌Ăяo����p�B
	//!       ������OnResize()���\�b�h���Ăяo���B
	//
	void ResizeEvent(UINT width, UINT height);

	//
	//! @brief �}�E�X�C�x���g����
	//!
	//! @note �E�B���h�E�v���V�[�W������̌Ăяo����p�B
	//!       ������OnMouse()���\�b�h���Ăяo���B
	//
	void MouseEvent(
		int x,
		int y,
		int wheelDelta,
		bool isLeftButtonDown,
		bool isRightButtonDown,
		bool isMiddleButtonDown,
		bool isSideButton1Down,
		bool isSideButton2down
	);

	//
	//! @brief �E�B���h�E�v���V�[�W��
	//
	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);
};