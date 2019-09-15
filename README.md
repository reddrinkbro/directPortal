# Portal
## 게임 소개
* 밸브 코퍼래이션이 제작한 Portal를 DirectX 9버전을 이용하여 모작한 게임입니다.
* 포탈건을 이용하여 맵을 돌아 다닐수 있습니다.
* 각 맵마다 존재하는 간단한 퍼즐을 풀어 마지막 엔딩까지 도달하는 게임입니다.

## 조작법
* 플레이어 이동: W 위쪽, A 왼쪽, D 오른쪽, S 아래쪽
* 상호작용 : E
* 점프 : Space
* 파랑 포탈 설치 : 왼쪽 마우스 버튼
* 주황 포탈 설치 : 오른쪽 마우스 버튼
* 메뉴 : ESC


## 개발 언어 및 환경
- OS : Windows 10 64bit 
- TOOLS : Visual studio 2015
- Language : C++

## 주요 클래스 설명
#### player
플레이어를 당담하는 클래스입니다. 조작법은 W,A,S,D 이며 상호작용은 E키를 통해 할 수 있습니다.

#### portal
포탈은 플레이어가 포탈건을 먹었을시 양쪽 마우스 버튼을 통해서 벽에 설치할 수 있습니다.

포탈은 모두가 설치되어 있을때는 서로 반대 위치로 플레이어를 이동시킵니다.

하나가 설치되어 있을때는 이동할 수 없습니다.

포탈은 벽의 방향에 따라 회전을 시켜서 설치됩니다.

포탈은 플레이어 뿐 아니라 큐브도 이동을 시킬 수 있습니다.

#### physicsManager
물리를 담당하는 클래스입니다.

Ray 충돌이나 물체 간의 충돌을 구현했습니다.

Ray는 시작위치와 방향을 정의하면 생성되는 직선을 의미합니다. 그래서 이 직선이 구, 박스와 충돌했는지 검사를 합니다.

물체 간의 충돌은 OBB 충돌을 통해서 검사합니다.

OBB 충돌은 물체에 존재하는 경계상자가 있는데 이 상자가 물체와 같은 회전을 하는 상자이다. 서로 다른 물체들의 경계상자가 충돌했는지 확인하여 검사하는 방법입니다.

#### panelMover
맵에 존재하는 panelmover를 수행하는 클래스입니다.

두 개의 위치를 서로 보간을 통해서 반복하여 움직이도록 구현을 했습니다.

#### laserPoint
레이저를 담당하는 클래스입니다.

여러 방법이 있겠지만 저는 3개의 텍스처를 * 형식으로 길게 늘어뜨려서 레이저를 구현했습니다.






