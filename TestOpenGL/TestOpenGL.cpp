#include "stdafx.h"
#include <gl\gl.h>			// ���Ŀ�
#include <gl\glu.h>			// ʵ�ÿ�
#include <gl\glaux.h>		// ������

//������Ƥ������ɫ
#define MYCOLOR 0.988f, 0.9f, 0.788f

HDC			hDC=NULL;		// ������ɫ������
HGLRC		hRC=NULL;		// ˽��GDI�豸������
HWND		hWnd=NULL;		// ���洰�ھ��
HINSTANCE	hInstance;		// ��������ʵ��

bool	keys[256];			// ���ڼ������̵�����
bool	active = true;		// ���ڵĻ��־��ȱʡΪtrue
bool	fullscreen = true;	// ȫ����־ȱʡ�趨��ȫ��ģʽ
bool	wire = false;

int nFullWidth;
int nFullHeight;

////////////////////////////////////////////////////////////////////////////////////////////////
//Զ������
float G_fDistance;

//ˮƽ�ƶ�����
float G_fDistancce_horizon;

//��ֱ�ƶ�����
float G_fDistancce_vertical;

//ˮƽ��ת�Ƕ� 
float G_fAngle_horizon;

//��ֱ��ת�Ƕ� 
float G_fAngle_vertical;

//���ϱ���ת�Ƕ�
float G_fRArm_Ang;

//���ϱ���ת�Ƕ�
float G_fLArm_Ang;

//�Ҵ�����ת�Ƕ�
float G_fRLeg_Ang;

//�������ת�Ƕ�
float G_fLLeg_Ang;

//��С����ת�Ƕ�
float G_fRCalf_Ang;

//��С����ת�Ƕ�
float G_fLCalf_Ang;

//���±���ת�Ƕ�
float G_fLDArm_Ang;

//���±���ת�Ƕ�
float G_fRDArm_Ang;

//���ϱ���ת�Ƕ�
float G_fLArmF_Ang;

//���ϱ���ת�Ƕ�
float G_fRArmF_Ang;

//�Դ���ת�Ƕ�
float G_fHead_Ang;

//�ұ�ˮƽƫת��
float G_fRLeg_Horizon;

//���ˮƽƫת��
float G_fLLeg_Horizon;

//���սǶ�
float G_fLightx;

//���ղ���
float G_vLitAmbient[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

float G_vLitDiffuse[4] = { 0.8f, 0.75f, 0.6f, 1.0f };

float G_vLitSpecular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

float G_vLitPosition[4] = { 5.0f, 0.0f, 5.0f, 1.0f };

float G_vMaterialSpecu[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

////////////////////////////////////////////////////////////////////////////////////////////////
//��������
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//��ʼ��
void myinit(void);

//�ı���״
void myReshape(GLsizei w, GLsizei h);

//��ͼ����
int DrawGLScene(GLvoid);

//�ƽ�
void inline MoveNear(void);

//��Զ
void inline MoveFar(void);

//����
void inline MoveLeft(void);

//����
void inline MoveRight(void);

//����
void inline MoveUp(void);

//����
void inline MoveDown(void);

//��ת
void inline LeftRotate(void);

//��ת
void inline RightRotate(void);

//����ת��
void inline UpRotate(void);

//����ת��
void inline DownRotate(void);

//���ϱ�����ת��
void inline RotateLeftArmDown(void);

//���ϱ�����ת��
void inline RotateLeftArmUp(void);

//���ϱ�����ת��
void inline RotateRightArmDown(void);

//���ϱ�����ת��
void inline RotateRightArmUp(void);

//�����������ת
void inline RotateLeftLegDown(void);

//�����������ת
void inline RotateLeftLegUp(void);

//�Ҵ���������ת
void inline RotateRightLegDown(void);

//�Ҵ���������ת
void inline RotateRightLegUp(void);

//��С��������ת
void inline RotateRightCalfDown(void);

//��С��������ת
void inline RotateRightCalfUp(void);

//��С��������ת
void inline RotateLeftCalfDown(void);

//��С��������ת
void inline RotateLeftCalfUp(void);

//���û����˵�״̬Ϊ��ʼ״̬
void inline Reset(void);

//���±�������ת
void inline RotateLeftDArmDown(void);

//���±�������ת
void inline RotateLeftDArmUp(void);

//���±�������ת
void inline RotateRightDArmDown(void);

//���±�������ת
void inline RotateRightDArmUp(void);

//��������
void inline LightMoveLeft(void);

//��������
void inline LightMoveRight(void);

//�����ǰ��ת
void inline RotateLeftArmForward(void);

//��������ת
void inline RotateLeftArmBack(void);

//�ұ���ǰ��ת
void inline RotateRightArmForward(void);

//�ұ������ת
void inline RotateRightArmBack(void);

//�Դ���ת
void inline RotateHeadLeft(void);

//�Դ���ת
void inline RotateHeadRight(void);

////////////////////////////////////////////////////////////////////////////////////////////////

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// ���ò���ʼ��GL���ڴ�С
{
	if (height==0)										// ��ֹ����
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// ���õ�ǰ���ӿ�

	glMatrixMode(GL_PROJECTION);						// ѡ��ͶӰ����
	glLoadIdentity();									// ����ͶӰ����

	// ���㴰�ڵ���۱���
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// ѡ��ģ�͹۲����
	glLoadIdentity();									// ����ģ�͹۲����
}

int InitGL(GLvoid)										// �˴���ʼ��OpenGL������������
{
	Reset();											// ���û����˵�״̬
	glShadeModel(GL_SMOOTH);							// ������Ӱƽ��
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// ��ɫ����
	glClearDepth(1.0f);									// ������Ȼ���
	glEnable(GL_DEPTH_TEST);							// ������Ȳ���
	glDepthFunc(GL_LEQUAL);								// ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// ������ϸ��͸������
	return true;										// ��ʼ�� OK
}

//��ͼ����
int DrawGLScene(GLvoid)									
{
	//���������Ļ����ɫ���������Ļ����Ȼ���
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	G_vLitPosition[0] = 5.0f + G_fLightx;

	////////////////////////////////////////////////	
	//���ù���
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_AMBIENT, G_vLitAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, G_vLitDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, G_vLitSpecular);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 3.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, G_vLitPosition);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//�򿪹���
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	////////////////////////////////////////////////
	//�任����������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//����������Z��ƽ��-G_fDistance (ʹ��������λ�������ǰ��)
	glTranslatef(G_fDistancce_horizon, G_fDistancce_vertical, -G_fDistance);
//	glTranslatef(G_fDistancce_horizon, 0.0, 0.0);
//	glTranslatef(0.0, G_fDistancce_vertical, 0.0);
	glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);

	////////////////////////////////////////////////////////////////////////////////////////////////
	//�Դ�
glPushMatrix(); 
	glTranslatef(0, 1, 0);
	glRotatef(G_fHead_Ang, 0, 1, 0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireSphere(0.5);
	else
		auxSolidSphere(0.5);

	////////////////////////////////////////////////////////////////////////////////////////////////
	//����
glPushMatrix();
	glTranslatef(0, -0.1f, 0.5f);
	glColor3f(0.973f, 0.973f, 1.0f);
	if(wire)
		auxWireBox(0.05, 0.15, 0.05); 
	else
		auxSolidBox(0.05, 0.15, 0.05); 

	////////////////////////////////////////////////////////////////////////////////////////////////
	//���
	glTranslatef(0, -0.2f, -0.1f);	
	glColor3f(1.0f, 0.753f, 0.796f);
	if(wire)
		auxWireBox(0.1f, 0.02f, 0.05f);
	else
		auxSolidBox(0.1f, 0.02f, 0.05f);

	////////////////////////////////////////////////////////////////////////////////////////////////
	//�۾�
glPushMatrix();
	glColor3f(0.118f, 0.565f, 1.0f);
	glTranslatef(-0.15f, 0.4f, 0.07f);
	if(wire)
		auxWireSphere(0.05);
	else
		auxSolidSphere(0.05); 
	glTranslatef(0.3f, 0.0f, 0.0f);
	if(wire)
		auxWireSphere(0.05);
	else
		auxSolidSphere(0.05); 
glPopMatrix();
glPopMatrix();
glPopMatrix();
	
glPushMatrix(); 
	////////////////////////////////////////////////////////////////////////////////////////////////
	//����
	//glTranslatef(0.0f, 0.0f, -0.2f);
	glRotatef(90, 0, 1, 0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireIcosahedron(0.75);
	else
		auxSolidIcosahedron(0.75);

glPopMatrix();
glPushMatrix();

	////////////////////////////////////////////////////////////////////////////////////////////////
	//����
	glTranslatef(0.65f, 0.0f, 0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireSphere(0.1);
	else
		auxSolidSphere(0.1);
	glRotatef(G_fRArmF_Ang, 1, 0, 0); 
	glRotatef(G_fRArm_Ang + 180, 0, 0, -1); 
	glTranslatef(0, -0.5, 0);
	glColor3f(MYCOLOR);

	if(wire)
		auxWireCylinder(0.1, 0.5);
	else
		auxSolidCylinder(0.1, 0.5);
	glTranslatef(0.0, 1.0, 0.0);
	glColor3f(MYCOLOR);

	if(wire)
		auxWireSphere(0.1);
	else
		auxSolidSphere(0.1);

	glRotatef(-G_fRArm_Ang + G_fRDArm_Ang, 0, 0, -1); //��̧�����±�
	glTranslatef(0, -0.5, 0);
	glColor3f(MYCOLOR);

	if(wire)
		auxWireCylinder(0.1, 0.5);
	else
		auxSolidCylinder(0.1, 0.5);
	glTranslatef(0, 1, 0);
	glColor3f(MYCOLOR);

	if(wire)
	{
		auxWireSphere(0.1);
		auxWireOctahedron(0.135);
	}
	else
	{
		auxSolidSphere(0.1);
		auxSolidOctahedron(0.135);
	}
glPopMatrix();
	
glPushMatrix();

	////////////////////////////////////////////////////////////////////////////////////////////////
	//����
	glColor3f(MYCOLOR);
	glTranslatef(-0.65f, 0.0f, 0);
	if(wire)
		auxWireSphere(0.1);
	else
		auxSolidSphere(0.1);
	glRotatef(G_fLArmF_Ang, 1, 0, 0); //ǰ��̧�ֱ�
	glRotatef(G_fLArm_Ang - 180, 0, 0, -1); //��̧�����ϱ�
	glTranslatef(0, -0.5, 0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireCylinder(0.1, 0.5);
	else
		auxSolidCylinder(0.1, 0.5);
	glTranslatef(0.0, 1.0, 0.0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireSphere(0.1);
	else
		auxSolidSphere(0.1);
	glRotatef(-G_fLArm_Ang + G_fLDArm_Ang, 0, 0, -1); //��̧�����±�
	glTranslatef(0, -0.5, 0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireCylinder(0.1, 0.5);
	else
		auxSolidCylinder(0.1, 0.5);
	glTranslatef(0, 1, 0);
	glColor3f(MYCOLOR);
	if(wire)
	{
		auxWireSphere(0.1);
		auxWireOctahedron(0.135);
	}
	else
	{
		auxSolidSphere(0.1);
		auxSolidOctahedron(0.135);
	}
glPopMatrix();
	
glPushMatrix();
	////////////////////////////////////////////////////////////////////////////////////////////////
	//����
	glTranslatef(0.25f, -0.6f, 0.0f);
	glRotatef(G_fRLeg_Horizon, 0, 0, -1);
	glRotatef(180 + G_fRLeg_Ang, 1.0f, 0.0f, 0.0f); //̧�Ҵ���
	glColor3f(MYCOLOR);
	if(wire)
		auxWireSphere(0.15);
	else
		auxSolidSphere(0.15);
	glTranslatef(0, -0.18f, 0);
	glColor3f(MYCOLOR);

	if(wire)
		auxWireCylinder(0.15, 0.8);
	else
		auxSolidCylinder(0.15, 0.8);
	glTranslatef(0, 1, 0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireSphere(0.15);
	else
		auxSolidSphere(0.15);
	glRotatef(-G_fRLeg_Ang + G_fRCalf_Ang, 1.0, 0.0, 0.0); //̧��С��
	glTranslatef(0, -0.5, 0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireCylinder(0.15, 0.5);
	else
		auxSolidCylinder(0.15, 0.5);
	glTranslatef(0, 1, 0);
	glColor3f(MYCOLOR);
	if(wire)
	{
		auxWireSphere(0.15);
		auxWireOctahedron(0.202);
	}
	else
	{
		auxSolidSphere(0.15);
		auxSolidOctahedron(0.202);
	}
	glRotatef(180, 0, 1, 0);

	if(wire)
	{
		auxWireCone(0.15, 0.15);
		auxWireTetrahedron(0.3);
	}
	else
	{
		auxSolidCone(0.15, 0.15);
		auxSolidTetrahedron(0.3);
	}
glPopMatrix();
	
glPushMatrix();
	////////////////////////////////////////////////////////////////////////////////////////////////
	//����
	glTranslatef(-0.25f, -0.6f, 0.0f);
	glRotatef(G_fLLeg_Horizon, 0, 0, -1);
	glRotatef(180 + G_fLLeg_Ang, 1.0f, 0.0f, 0.0f); //̧�����
	glColor3f(MYCOLOR);
	if(wire)
		auxWireSphere(0.15);
	else
		auxSolidSphere(0.15);
	glTranslatef(0, -0.18f, 0);
	glColor3f(MYCOLOR);

	if(wire)
		auxWireCylinder(0.15, 0.8);
	else
		auxSolidCylinder(0.15, 0.8);
	glTranslatef(0, 1, 0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireSphere(0.15);
	else
		auxSolidSphere(0.15);
	glRotatef(-G_fLLeg_Ang + G_fLCalf_Ang, 1.0, 0.0, 0.0); //̧��С��
	glTranslatef(0, -0.5, 0);
	glColor3f(MYCOLOR);
	if(wire)
		auxWireCylinder(0.15, 0.5);
	else
		auxSolidCylinder(0.15, 0.5);
	glTranslatef(0, 1, 0);
	glColor3f(MYCOLOR);
	if(wire)
	{
		auxWireSphere(0.15);
		auxWireOctahedron(0.202);
	}
	else
	{
		auxSolidSphere(0.15);
		auxSolidOctahedron(0.202);
	}
	glRotatef(90, 0, 1, 0);

	if(wire)
	{
		auxWireCone(0.15, 0.15);
		auxWireTetrahedron(0.3);
	}
	else
	{
		auxSolidCone(0.15, 0.15);
		auxSolidTetrahedron(0.3);
	}
glPopMatrix();

	auxSwapBuffers();

	return true;										// һ��OK
}

GLvoid KillGLWindow(GLvoid)								// �������ٴ���
{
	if (fullscreen)										// �Ƿ�ȫ��ģʽ?
	{
		ChangeDisplaySettings(NULL,0);					// �л�������
		ShowCursor(true);								// ��ʾ���ָ��
	}

	if (hRC)											// ����ɫ��������?
	{
		if (!wglMakeCurrent(NULL,NULL))					// �����ܷ��ͷ�DC��RC������?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// �ܷ�ɾ��RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// �ܷ��ͷ�DC?
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// �ܷ����ٴ���?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// �ܷ�ע����?
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (true) Or Windowed Mode (false)	*/
 
bool CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// �������ƥ��Ľ��
	WNDCLASS	wc;						// ������ṹ
	DWORD		dwExStyle;				// ��չ���ڷ��
	DWORD		dwStyle;				// ���ڷ��
	RECT		WindowRect;				// ȡ�þ��ε����ϽǺ����½ǵ�����ֵ
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// ����ȫ��ȫ�����

	hInstance			= GetModuleHandle(NULL);				// ȡ�ô��ڵ�ʵ��
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// �ƶ�ʱ�ػ�����Ϊ����ȡ��DC.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc������Ϣ
	wc.cbClsExtra		= 0;									// �޶��ⴰ������
	wc.cbWndExtra		= 0;									// �޶��ⴰ������
	wc.hInstance		= hInstance;							// ����ʵ��
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// װ��ȱʡͼ��
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// װ�����ָ��
	wc.hbrBackground	= NULL;									// GL����Ҫ����
	wc.lpszMenuName		= NULL;									// ����Ҫ�˵�
	wc.lpszClassName	= "OpenGL";								// �趨������

	if (!RegisterClass(&wc))									// ����ע�ᴰ����
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											
	}
	
	if (fullscreen)												// ����ȫ��ģʽ?
	{
		DEVMODE dmScreenSettings;								// �豸ģʽ
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// ȷ���ڴ����
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// ����������ʾģʽ�����ؽ����ע: CDS_FULLSCREEN ��ȥ��״̬����
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// ��ģʽʧ�ܣ��ṩ����ѡ��˳����ڴ���������
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=false;		// ѡ�񴰿�ģʽ
			}
			else
			{
				// ����û�ѡ���˳���������Ϣ���ڸ�֪�û����򽫽�����
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return false;									// Return false
			}
		}
	}

	if (fullscreen)												// �Դ���ȫ��ģʽ?
	{
		dwExStyle=WS_EX_APPWINDOW;								// ��չ������
		dwStyle=WS_POPUP;										// ������
		ShowCursor(false);										// �������ָ��
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// ��չ������
		dwStyle=WS_OVERLAPPEDWINDOW;							// ������
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// ȡ���豸��������ô?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	ShowWindow(hWnd,SW_SHOW);						// ��ʾ����
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// ���ü��̵Ľ���������
	ReSizeGLScene(width, height);					// ����͸��GL��Ļ

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	return true;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=true;						// Program Is Active
			}
			else
			{
				active=false;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As true
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As false
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// ʵ��
					HINSTANCE	hPrevInstance,		// ǰһ��ʵ��
					LPSTR		lpCmdLine,			// �����в���
					int			nCmdShow)			// ������ʾ״̬
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	nFullWidth = GetSystemMetrics(SM_CXSCREEN);   
	nFullHeight = GetSystemMetrics(SM_CYSCREEN);   

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"ȫ��������?", "ѡ������ģʽ",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=false;							// ����ģʽ
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("������",nFullWidth,nFullHeight,32,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}
	Reset();

	while(!done)									
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClearDepth(1.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				done=true;							// If So done=true
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
					glClearDepth(1.0);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					done=true;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=false;					// If So Make Key false
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("������",nFullWidth,nFullHeight,32,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}

			if (keys[VK_F2])
			{
				keys[VK_F2] = false;
				wire = !wire;
			}

			if (keys[VK_SPACE])						
			{
				keys[VK_SPACE] = false;
				Reset();
			}

			if(keys[VK_ADD])
			{
				keys[VK_ADD] = false;
				MoveNear();
			}

			if(keys[VK_SUBTRACT])
			{
				keys[VK_SUBTRACT] = false;
				MoveFar();
			}

			int kg = 0;
			if(keys[VK_INSERT])
				kg += 1;
			if(keys[VK_HOME])
				kg += 2;
			if(keys[VK_DELETE])
				kg += 5;
			if(keys[VK_END])
				kg += 11;
			if(keys[VK_CONTROL])
				kg += 23;
			if(keys[VK_SHIFT])
				kg += 43;
			if(keys[VK_PRIOR])
				kg += 87;
			if(keys[VK_NEXT])
				kg += 173;

			if (keys[VK_LEFT])				
			{
				keys[VK_LEFT] = false;
				switch(kg)
				{
				case 0	: LeftRotate();				break;
				case 1	: RotateLeftDArmUp();		break;
				case 2	: RotateRightDArmDown();	break;
				case 5	: RotateLeftCalfDown();		break;
				case 11	: RotateRightCalfDown();	break;
				case 23 : MoveLeft();				break;
				case 43	: LightMoveLeft();			break;
				case 87	: RotateLeftArmForward();	break;
				case 173: RotateLeftArmBack();		break;
				default : break;
				}
			}

			if (keys[VK_RIGHT])
			{
				keys[VK_RIGHT] = false;
				switch(kg)
				{
				case 0	: RightRotate();			break;
				case 1	: RotateLeftDArmDown();		break;
				case 2	: RotateRightDArmUp();		break;
				case 5	: RotateLeftCalfUp();		break;
				case 11	: RotateRightCalfUp();		break;
				case 23 : MoveRight();				break;
				case 43	: LightMoveRight();			break;
				case 87	: RotateRightArmForward();	break;
				case 173: RotateRightArmBack();		break;
				default : break;
				}
			}

			if (keys[VK_UP])
			{
				keys[VK_UP] = false;
				switch(kg)
				{
				case 0	: UpRotate();				break;
				case 1	: RotateLeftArmUp();		break;
				case 2	: RotateRightArmUp();		break;
				case 5	: RotateLeftLegUp();		break;
				case 11	: RotateRightLegUp();		break;
				case 23 : MoveUp();					break;
				default : break;
				}
			}

			if (keys[VK_DOWN])
			{
				keys[VK_DOWN] = false;
				switch(kg)
				{
				case 0	: DownRotate();				break;
				case 1	: RotateLeftArmDown();		break;
				case 2	: RotateRightArmDown();		break;
				case 5	: RotateLeftLegDown();		break;
				case 11	: RotateRightLegDown();		break;
				case 23 : MoveDown();				break;
				default : break;
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

//�ƽ�
void inline MoveNear(void)
{
	G_fDistance -= 0.1f;
}

//��Զ
void inline MoveFar(void)
{
	G_fDistance += 0.1f;
}

//����
void inline MoveLeft(void)
{
	G_fDistancce_horizon -= 0.02f;
}

//����
void inline MoveRight(void)
{
	G_fDistancce_horizon += 0.02f;
}

//����
void inline MoveUp(void)
{
	G_fDistancce_vertical += 0.05f;
}

//����
void inline MoveDown(void)
{
	G_fDistancce_vertical -= 0.05f;
}

//��ת
void inline LeftRotate(void) 
{
	G_fAngle_horizon -= 5;
}

//��ת
void inline RightRotate(void) 
{
	G_fAngle_horizon += 5;
}

//����ת��
void inline UpRotate(void)
{
	G_fAngle_vertical -= 5;
}

//����ת��
void inline DownRotate(void)
{
	G_fAngle_vertical += 5;
}

//���ϱ�����ת��
void inline RotateLeftArmDown(void) 
{
	if (G_fLArm_Ang > 0)
	{
		G_fLArm_Ang -= 3;
	}
	if (G_fLArm_Ang < G_fLDArm_Ang)
	{
		G_fLDArm_Ang = G_fLArm_Ang;
	}
}

//���ϱ�����ת��
void inline RotateLeftArmUp(void)
{
	if (G_fLArm_Ang < 135)
	{
		G_fLArm_Ang += 3;
	}
	if (G_fLArm_Ang - 135 > G_fLDArm_Ang)
	{
		G_fLDArm_Ang = G_fLArm_Ang - 135;
	}
}

//���ϱ�����ת��
void inline RotateRightArmDown(void)
{
	if (G_fRArm_Ang < 0)
	{
		G_fRArm_Ang += 3;
	}
	if (G_fRArm_Ang > G_fRDArm_Ang)
	{
		G_fRDArm_Ang = G_fRArm_Ang;
	}
}

//���ϱ�����ת��
void inline RotateRightArmUp(void)
{
	if (G_fRArm_Ang > -135)
	{
		G_fRArm_Ang -= 3;
	}
	if (G_fRArm_Ang + 135 < G_fRDArm_Ang)
	{
		G_fRDArm_Ang = G_fRArm_Ang + 135;
	}
}

//�����������ת
void inline RotateLeftLegDown(void)
{
	if (G_fLLeg_Ang < 90)
		G_fLLeg_Ang += 3;
	if (G_fLLeg_Ang > G_fLCalf_Ang)
		G_fLCalf_Ang = G_fLLeg_Ang;
}

//�����������ת
void inline RotateLeftLegUp(void)
{
	if (G_fLLeg_Ang > -135)
		G_fLLeg_Ang -= 3;
	if (G_fLCalf_Ang > G_fLLeg_Ang + 135)
		G_fLCalf_Ang = G_fLLeg_Ang + 135;
}

//�Ҵ���������ת
void inline RotateRightLegDown(void)
{
	if (G_fRLeg_Ang < 90)
		G_fRLeg_Ang += 3;
	if (G_fRLeg_Ang > G_fRCalf_Ang)
		G_fRCalf_Ang = G_fRLeg_Ang;
}

//�Ҵ���������ת
void inline RotateRightLegUp(void)
{
	if (G_fRLeg_Ang > -135)
		G_fRLeg_Ang -= 3;
	if (G_fRCalf_Ang > G_fRLeg_Ang + 135)
		G_fRCalf_Ang = G_fRLeg_Ang + 135;
}

//��С��������ת
void inline RotateRightCalfDown(void)
{
	if (G_fRCalf_Ang < G_fRLeg_Ang + 135)
		G_fRCalf_Ang += 3;
}

//��С��������ת
void inline RotateRightCalfUp(void)
{
	if (G_fRCalf_Ang > G_fRLeg_Ang)
		G_fRCalf_Ang -= 3;
}

//��С��������ת
void inline RotateLeftCalfDown(void)
{
	if (G_fLCalf_Ang < G_fLLeg_Ang + 135)
		G_fLCalf_Ang += 3;
}

//��С��������ת
void inline RotateLeftCalfUp(void)
{
	if (G_fLCalf_Ang > G_fLLeg_Ang)
		G_fLCalf_Ang -= 3;
}

//���û����˵�״̬Ϊ��ʼ״̬
void inline Reset(void) 
{
	G_fDistance = 5.5f;
	G_fDistancce_horizon = 0.0f;
	G_fDistancce_vertical = 0.0f;
	G_fAngle_horizon = 0.0f;
	G_fAngle_vertical = 0.0f;
	G_fRArm_Ang = 0.0f;
	G_fLArm_Ang = 0.0f;
	G_fRLeg_Ang = 0.0f;
	G_fLLeg_Ang = 0.0f;
	G_fRCalf_Ang = 0.0f;
	G_fLCalf_Ang = 0.0f;
	G_fLDArm_Ang = 0.0f;
	G_fRDArm_Ang = 0.0f;
	G_fLArmF_Ang = 0.0f;
	G_fRArmF_Ang = 0.0f;
	G_fHead_Ang = 0.0f;
	G_fRLeg_Horizon = 0.0f;
	G_fLLeg_Horizon = 0.0f;
	G_fLightx = 0.0f;
}

//���±�������ת
void inline RotateLeftDArmDown(void)
{
	if (G_fLArm_Ang - G_fLDArm_Ang < 135)
		G_fLDArm_Ang -= 3;
}

//���±�������ת
void inline RotateLeftDArmUp(void) {
	if (G_fLArm_Ang > G_fLDArm_Ang)
		G_fLDArm_Ang += 3;
}

//���±�������ת
void inline RotateRightDArmDown(void)
{
	if (G_fRDArm_Ang - G_fRArm_Ang < 135)
		G_fRDArm_Ang += 3;
}

//���±�������ת
void inline RotateRightDArmUp(void)
{
	if (G_fRArm_Ang < G_fRDArm_Ang)
		G_fRDArm_Ang -= 3;
}

//��������
void inline LightMoveLeft(void)
{
	if (G_fLightx > -120)
		G_fLightx -= 3;
}

//��������
void inline LightMoveRight(void)
{
	if (G_fLightx < 120)
		G_fLightx += 3;
}

//�����ǰ��ת
void inline RotateLeftArmForward(void)
{
	if (G_fLArmF_Ang > -180)
		G_fLArmF_Ang -= 3;
}

//��������ת
void inline RotateLeftArmBack(void)
{
	if (G_fLArmF_Ang < 45)
		G_fLArmF_Ang += 3;
}

//�ұ���ǰ��ת
void inline RotateRightArmForward(void)
{
	if (G_fRArmF_Ang > -180)
		G_fRArmF_Ang -= 3;
}

//�ұ������ת
void inline RotateRightArmBack(void)
{
	
	if (G_fRArmF_Ang < 45)
		G_fRArmF_Ang += 3;
}

//�Դ���ת
void inline RotateHeadLeft(void)
{
	if (G_fHead_Ang < 90)
		G_fHead_Ang += 3;
}

//�Դ���ת
void inline RotateHeadRight(void)
{
	if (G_fHead_Ang > -90)
		G_fHead_Ang -= 3;
}
