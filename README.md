# GoKill

## 작성 버전
언리얼 5.5.4
비주얼 스튜디오 2022

## 세팅
### MetaXR 플러그인 추가

1. 이하 URL 에서 아래 3가지 다운로드
https://dev.epicgames.com/community/learning/tutorials/PYP7/unreal-engine-5-5-x-for-meta-quest-vr

Unreal Engine 5 Integration v72.1 (Meta XR Plugin)
Unreal Engine 5 Platform v72 (Meta XR Platform)
Meta XR Simulator v72

2. 프로젝트 폴더에 Plugins 폴더 추가
3. Unreal Engine 5 Integration v72.1 (MetaXR 폴더), Unreal Engine 5 Platform v72 (MetaXRPlatform 폴더) 압축 풀고 Plugins 폴더에 넣기

4. Meta XR Simulator v72 압축풀기
5. 언리얼 프로젝트 실행 후 Project Settings -> Plugins -> Meta XR -> Meta XR Simulator JSON File 에서 압축 해제한 package 에서 MetaXRSimulator -> meta_openxr_simulator.json 파일 선택

### MetaXR 시뮬레이터 사용
- VR 기기 없이 VR 시뮬레이터 사용
Viewport 윗줄 -> Meta XR Simulator -> Meta XR Simulator 체크

- VR 실행
실행 햄버거 버튼 -> VR Preview

### 서버 세팅
프로젝트 폴더\Plugins\Access 에 있는 Binaries 제거 후 Generate Visual Studio project files

## 테스트 서버 실행
프로젝트 폴더\Plugins\server\TestServer_2503_ver121.exe 실행하고 프로젝트 실행