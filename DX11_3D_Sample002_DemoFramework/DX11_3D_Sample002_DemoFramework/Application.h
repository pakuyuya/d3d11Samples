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
	//! @brief コンストラクタ
	//
	Application();

	
	//
	//! @brief コンストラクタ
	//!
	//! @param [in] title タイトル名
	//
	Application(LPSTR title);

	//
	//! @brief デストラクタ
	//
	virtual ~Application();

	//
	//! @brief アプリケーション実行
	//
	void Run();


	// -------------------------- Protected Fields ----------------------
protected:
	HINSTANCE m_hInst;					//!< インスタンスハンドル
	HWND m_hWnd;						//!< ウィンドウハンドル
	D3D_DRIVER_TYPE m_DriverType;		//!< ドライバータイプ
	D3D_FEATURE_LEVEL m_FeatureLevel;	//!< 機能レベル
	UINT m_MultiSampleCount;			//!< マルチサンプリングカウント数
	UINT m_MultiSampleQuality;			//!< マルチサンプル品質
	UINT m_SwapChainCount;				//!< スワップチェインバッファ数
	DXGI_FORMAT m_SwampChainFormat;		//!< スワップチェインバッファのフォーマット
	DXGI_FORMAT m_DepthStencilFormat;	//!< 深度ステンシルバッファのフォーマット
	ID3D11Device* m_pDevice;				//!< デバイス
	ID3D11DeviceContext* m_pDeviceContext;	//!< デバイスコンテキスト
	IDXGISwapChain* m_pSwapChain;			//!< スワップチェイン
	RenderTarget2D m_RenderTarget2D;		//!< 描画ターゲット
	DepthStencilBuffer m_DepthStencilBuffer;	//!< 深度ステンシルバッファ
	ID3D11RasterizerState* m_pRasterizerState;	//!< ラスタライザステート
	ID3D11DepthStencilState* m_pDepthStencilState;	//!< 深度ステンシルステート
	ID3D11BlendState* m_pBlendState;			//!< ブレンドステート
	FLOAT m_BlendFactor[4];	//!< ブレンドファクター
	UINT m_SampleMask;		//!< サンプルマスク
	UINT m_StencilRef;		//!< ステンシル参照
	FLOAT4 m_ClearColor;	//!< 背景のクリアカラー
	UINT m_Width;			//!< 画面幅
	UINT m_Height;			//!< 画面高さ
	FLOAT m_AspectRatio;	//!< 画面のアスペクト比
	LPSTR m_Title;			//!< タイトル
	Timer m_Timer;			//!< タイマー

	// -------------------------- Protected Methods ----------------------

	//
	//! @brief 初期化処理
	//! 
	//! @note 派生クラスにて実装を行う
	//
	virtual bool OnInit();
	
	//
	//! @brief 終了時処理
	//!
	//! @note 派生クラスにて実装を行う
	//
	virtual void OnTerm();
	
	//
	//! @brief フレーム遷移時に実行する処理
	//!
	//! @param [in] time アプリケーション開始からの相対時間
	//! @param [in] elapsedTime 前のフレームからの経過時間
	//! @note 派生クラスにて実装を行う。
	//
	virtual void OnFrameMove(double time, double elapsedTime);

	//
	//! @brief フレーム描画時に実行する処理
	//!
	//! @param [in] time アプリケーション開始からの相対時間
	//! @param [in] elapsedTime 前のフレームからの経過時間
	//! @note 派生クラスにて実装を行う。
	//
	virtual void OnFrameRender(double time, double elapsedTime);

	//
	//! @brief リサイズ時に実行する処理
	//!
	//! @param [in] param リサイズパラメータ
	//! @note 派生クラスにて実装を行う。
	//
	virtual void OnResize(const ResizeEventParam& param);

	//
	//! @brief キーイベント通知時に実行する処理
	//!
	//! @param [in] param キーイベントパラメータ
	//! @note 派生クラスにて実装を行う。
	//
	virtual void OnKey(const KeyEventParam& param);

	//
	//! @brief マウスイベント通知時に実行する処理
	//!
	//! @param [in] param マウスイベントパラメータ
	//! @note 派生クラスにて実装を行う。
	//
	virtual void OnMouse(const MouseEventParam& param);

	//
	//! @brief 描画停止フラグを設定
	//!
	//! @param [in] isStopRendering 設定値
	//
	void SetStopRendering(bool isStopRendering);

	//
	//! @brief 描画停止フラグを取得
	//!
	//! @retval true 描画処理を呼び出さない
	//! @retval false 描画処理を呼び出す
	//
	bool IsStopRendering() const;

	//
	//! @brief フレームカウントを取得
	//!
	//! @return フレームカウント
	//
	DWORD GetFrameCount() const;

	//
	//! @brief FPS取得
	//!
	//! @return FPS
	//
	FLOAT GetFPS() const;


	// -------------------------- Private Fields ----------------------

	bool m_IsStopRendering;		//!< 描画停止フラグ
	DWORD m_FrameCount;			//!< フレームカウント
	FLOAT m_FPS;				//!< FPS
	DOUBLE m_LatestUpdateTime;	//!< 最後の更新時間

	
	// -------------------------- Private Methods ----------------------

	//! @note アクセス禁止
	Application(const Application&); 

	//! @note アクセス禁止
	void operator =(const Application&);
	

	//
	//! @brief アプリケーション初期化
	//!
	//! @return 成功可否
	//! @note 内部でInitWnd()、InitD3D()を呼び出し
	//
	bool InitApp();

	//
	//! @brief アプリケーション終了
	//!
	//! @note 内部でTermWnd() TermD3D()を呼び出し
	//
	void TermApp();

	//
	//! @brief ウィンドウ初期化
	//!
	//! @return 成功可否
	//
	bool InitWnd();

	//
	//! @brief ウィンドウ終了
	//
	void TermWnd();

	//
	//! @brief Direct3D初期化
	//
	bool InitD3D();

	//
	//! @brief Direct3D終了
	//
	void TermD3D();

	//
	//! @breif メインループ処理
	//
	void MainLoop();

	//
	//! @brief キーイベント処理
	//!
	//! @note ウィンドウプロシージャからの呼び出し専用。
	//!       内部でOnKey()メソッドを呼び出す。
	//
	void KeyEvent(UINT nChar, bool isKeyDown, bool isAltDown);

	//
	//! @brief リサイズイベント処理
	//!
	//! @note ウィンドウプロシージャからの呼び出し専用。
	//!       内部でOnResize()メソッドを呼び出す。
	//
	void ResizeEvent(UINT width, UINT height);

	//
	//! @brief マウスイベント処理
	//!
	//! @note ウィンドウプロシージャからの呼び出し専用。
	//!       内部でOnMouse()メソッドを呼び出す。
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
	//! @brief ウィンドウプロシージャ
	//
	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);
};