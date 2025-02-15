# 🎮 포트리스, 알카노이드 & 미로 탐색 프로젝트
_WinApi를 사용해 2D 충돌 감지 시스템을 활용한 포트리스(Fortress) 스타일 대전 게임, 알카노이드(Arkanoid) 게임, 그리고 미로 탐색 AI 구현_

---

## 📌 프로젝트 개요
이 프로젝트는 **이 프로젝트는 2D 공간에서 다양한 충돌 감지를 처리하는 시스템을 구현한 것입니다. 사각형, 원, 선분 등의 기본적인 충돌체(Collider)를 정의하고, 이들 간의 충돌 여부를 판별할 수 있도록 설계한 후 이를 활용해 충돌 감지 시스템과 경로 탐색 알고리즘을 활용한 2D 게임 및 시뮬레이션**을 목표로 합니다.  
포트리스(Fortress) 스타일의 **대포 대전 게임**,  
알카노이드(Arkanoid) 스타일의 **블록 깨기 게임**,  
그리고 **미로 탐색 AI**를 구현하였습니다.

---

## 충돌감지 시스템
- **충돌체(Collider) 시스템**  
  - `CircleCollider` (원형 충돌체)
  - `RectCollider` (사각형 충돌체)
  - `Line` (선형 객체)

- **충돌 감지 기능**  
  - 원-원 충돌 감지 (`CircleCollider`) : 두 원의 중심거리 비교
  - 원-사각형 충돌 감지 (`CircleCollider` vs `RectCollider`) :  가장 가까운 사각형의 점과 원 중심 간의 거리 비교  
  - 사각형-사각형 충돌 감지 (`RectCollider`) : 좌표 범위 비교  
  - 선-선 충돌 감지 (`Line`) : 두 선이 교차하는지 확인  

---

## 구현 프로젝트

### 🎯 1. 포트리스 스타일 대전 게임
- `←`, `→` 키: **대포 이동**
- `↑`, `↓` 키: **발사 각도 조정**
- `Space` 키: **탄환 발사**
- 탄환이 중력의 영향을 받아 포물선을 그리며 낙하
- 탄환이 상대 대포에 명중하면 게임 종료

✅ **주요 기능**
- 대포(Cannon) 조작 및 발사 시스템
- 탄환(Bullet) 충돌 감지 및 반사 처리
- 씬(Scene) 관리 및 객체 업데이트

🔥 **핵심 기술 및 구현**
```
void Bullet::Update()
{
    if (_isActive == false) return;

    _direction._y += _gravity;  // 중력 적용
    _col->_center += _direction * _speed; 

    // 벽 충돌 처리
    if (_col->_center._x > WIN_WIDTH || _col->_center._x < 0)
        _direction._x *= -1.0f;

    // 바닥 충돌 시 탄환 소멸
    if (_col->_center._y > WIN_HEIGHT)
    {
        _isActive = false;
        return;
    }

    _col->Update();
}

```
- 탄환이 중력의 영향을 받아 떨어짐
- 벽 충돌 시 반사 처리
- 바닥에 닿으면 소멸
  

---

### 🏓 2. 알카노이드(Arkanoid) 스타일 게임
- `←`, `→` 키: **패들(PlayerBar) 이동**
- `Space` 키: **공(Ball) 발사**
- 공이 벽에 충돌 시 반사
- 패들(PlayerBar)과 충돌 시 반사 방향 변경

✅ **주요 기능**
- 공(PlayerBall) 및 패들(PlayerBar) 충돌 감지
- 공의 속도 및 방향 업데이트
- 씬(Scene) 관리 및 게임 루프 구현

🔥 **핵심 기술 및 구현**
```
void PlayerBar::IsCollision(shared_ptr<PlayerBall> ball)
{
    if (_rect->IsCollision(ball->GetCircleCollider()))
    {
        Vector2 dir = ball->GetCircleCollider()->_center - _rect->_center;
        dir.Normalize();
        ball->SetDir(dir);
    }
}
```
- 공이 벽에 반사됨
- 패들(PlayerBar)과 충돌 시 반사 방향 변경

---

### 🏁 3. 미로 탐색 AI (Maze Solver)
- 랜덤으로 **미로를 생성**
- AI가 **다양한 경로 탐색 알고리즘을 이용하여 최단 경로를 찾음**
- **탐색 알고리즘**
  - 깊이 우선 탐색(DFS)
  - 너비 우선 탐색(BFS)
  - 다익스트라(Dijkstra) 알고리즘
  - A* (A-star) 알고리즘
  - 우측 손 법칙 (Right-Hand Rule)

✅ **주요 기능**
- `MazeScene`에서 **랜덤 미로 생성**
- `Player` AI가 미로를 탐색하여 출구를 찾음
- 알고리즘별 탐색 과정 시각화

🔥 **핵심 기술 및 구현**
```
void Maze::CreateMaze_Kruskal()
{
    // Kruskal 알고리즘을 이용한 미로 생성
    vector<Edge> edges;
    for (int y = 0; y < MAXCOUNT_Y; y++)
    {
        for (int x = 0; x < MAXCOUNT_X; x++)
        {
            if (x % 2 == 0 || y % 2 == 0) continue;

            if (x < MAXCOUNT_X - 2)
            {
                int randCost = rand() % 100;
                edges.push_back({Vector2(y,x), Vector2(y, x + 2), randCost});
            }

            if (y < MAXCOUNT_Y - 2)
            {
                int randCost = rand() % 100;
                edges.push_back({Vector2(y,x), Vector2(y + 2, x), randCost});
            }
        }
    }

    // 경로 연결
    std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2){ return e1.cost < e2.cost; });
    DisJointSet set(MAXCOUNT_X * MAXCOUNT_Y);

    for (auto& edge : edges)
    {
        int u = edge.u._y + edge.u._x * MAXCOUNT_X;
        int v = edge.v._y + edge.v._x * MAXCOUNT_X;

        if(set.FindLeader(u) == set.FindLeader(v)) continue;
        set.Merge(u,v);

        int x = ((int)edge.u._y + (int)edge.v._y) / 2;
        int y = ((int)edge.u._x + (int)edge.v._x) / 2;

        _blocks[y][x]->SetBlockType(Block::BlockType::ABLE);
    }
}

```
- 미로를 Kruskal 알고리즘으로 생성
- AI가 BFS, A* 등 다양한 탐색 알고리즘으로 최단 경로 탐색

---

## 📈 성과 및 학습한 점
✅ **충돌 감지 시스템 설계**  
   - `CircleCollider`, `RectCollider`, `Line`을 활용한 다양한 충돌 감지  
   - 객체 간 충돌 판별 및 반사 처리 알고리즘 구현  

✅ **객체지향 프로그래밍(OOP) 적용**  
   - `Scene`, `Collider`, `Cannon` 등 **계층적 구조 설계**  
   - `shared_ptr`, `weak_ptr` 활용하여 **메모리 관리 최적화**  

✅ **WinAPI 기반 2D 렌더링**  
   - `HDC`를 이용하여 `Ellipse()`, `Rectangle()`, `LineTo()` 등 **기본 GDI 함수 활용**  
   - `SelectObject()`를 사용하여 대포 및 탄환 색상 변경  

✅ **실시간 입력 처리**  
   - `GetAsyncKeyState()`를 활용한 **키 입력 감지**  
   - 탄환 발사 시 `std::find_if()`를 활용하여 **비활성화된 탄환 재사용**



