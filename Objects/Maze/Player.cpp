#include "pch.h"
#include "Player.h"

#include "Maze.h"
#include "Block.h"

Player::Player(shared_ptr<Maze> maze)
{
	_maze = maze;
}

Player::~Player()
{
	_maze = nullptr;
}

void Player::BeginPlay()
{
	_pos = _maze->GetStartPos();
	_maze->SetPlayerPos(_pos);

	_visited = vector<vector<bool>>(MAXCOUNT_Y, vector<bool>(MAXCOUNT_X, false));
	//Djikstra(_pos);
	AStart(_pos, _maze->GetEndPos());
}

void Player::RightHand()
{
	enum Direction
	{
		UP,
		LEFT,
		BOTTOM,
		RIGHT,

		DIR_COUNT = 4
	};

	Vector2 pos = _pos;
	_path.push_back(pos);
	Vector2 endPos = _maze->GetEndPos();

	Direction dir = Direction::BOTTOM;

	Vector2 frontPos[4] = 
	{
		Vector2 {0,-1},  
		Vector2 {-1,0}, 
		Vector2 {0,1}, 
		Vector2 {1,0},
	};

	while (true)
	{
		if(pos == endPos)
			break;

		int newDir = (dir - 1 + DIR_COUNT) % DIR_COUNT;
		Vector2 oldDirVector2 = frontPos[dir];
		Vector2 newDirVector2 = frontPos[newDir];

		Vector2 newPos = pos + newDirVector2; 
		Vector2 oldPos = pos + oldDirVector2; 

		if (Cango(newPos._y, newPos._x))
		{
			dir = static_cast<Direction>(newDir);
			pos += newDirVector2;
			_path.push_back(pos);
		}
	
		else if (Cango(oldPos._y, oldPos._x))
		{
			pos += oldDirVector2;
			_path.push_back(pos);
		}

		else
		{
			dir = static_cast<Direction>((dir + 1 + DIR_COUNT) % DIR_COUNT);
		}
	}

	stack<Vector2> s;

	for (int i = 0; i < _path.size() - 1; i++)
	{
		if (s.empty() == false && s.top() == _path[i + 1])
			s.pop();
		else
			s.push(_path[i]);
	}

	s.push(_path.back());

	_path.clear();

	while (true)
	{
		if(s.empty() == true)
			break;

		_path.push_back(s.top());
		s.pop();
	}

	std::reverse(_path.begin(), _path.end());
}

void Player::DFS(Vector2 here)
{
	Vector2 frontPos[8] =
	{
		Vector2 {0,-1}, 
		Vector2 {-1,0}, 
		Vector2 {0,1}, 
		Vector2 {1,0}, 

		Vector2 {-1,-1}, 
		Vector2 {-1,1}, 
		Vector2 {1,1}, 
		Vector2 {1,-1}, 
	};

	_visited[here._y][here._x] = true;
	_path.push_back(here);
	_maze->SetBlockType(here._y, here._x, Block::BlockType::FOOT_PRINT);

	for (int i = 0; i < 4; i++)
	{
		Vector2 there = here + frontPos[i];

		if(_visited[there._y][there._x]) continue;

		if(!Cango(there._y, there._x)) continue;

		if (there == _maze->GetEndPos())
		{
			_path.push_back(there);
			continue;
		}

		DFS(there);
	}
}

void Player::BFS(Vector2 start)
{
	Vector2 frontPos[8] =
	{
		Vector2 {-1,-1},
		Vector2 {-1,1}, 
		Vector2 {1,1}, 
		Vector2 {1,-1},

		Vector2 {0,-1}, 
		Vector2 {-1,0}, 
		Vector2 {0,1},
		Vector2 {1,0},
	};

	vector<vector<bool>> discovered = vector<vector<bool>>(MAXCOUNT_Y, vector<bool>(MAXCOUNT_X, false));
	vector<vector<Vector2>> parent = vector<vector<Vector2>>(MAXCOUNT_Y, vector<Vector2>(MAXCOUNT_X, Vector2(-1,-1)));

	Vector2 pos = start;
	Vector2 endPos = _maze->GetEndPos();

	discovered[start._y][start._x] = true;
	parent[start._y][start._x] = start;

	queue<Vector2> q;
	q.push(start);

	while (true)
	{
		if(q.empty()) break;

		Vector2 here = q.front();
		q.pop();
		if(here == endPos)
			break;

		for (int i = 0; i < 8; i++)
		{
			Vector2 there = here + frontPos[i];

			if(!Cango(there._y, there._x))
				continue;

			if(discovered[there._y][there._x] == true)
				continue;

			_maze->SetBlockType(there._y, there._x, Block::BlockType::FOOT_PRINT);

			q.push(there);
			discovered[there._y][there._x] = true;
			parent[there._y][there._x] = here;
		}
	}

	Vector2 check = endPos;
	_path.push_back(check);
	while (true)
	{
		if(check == start) break;

		check = parent[check._y][check._x];
		_path.push_back(check);
	}

	std::reverse(_path.begin(), _path.end());
}

void Player::Djikstra(Vector2 start)
{
	Vector2 frontPos[8] =
	{
		Vector2 {-1,-1}, 
		Vector2 {-1,1}, 
		Vector2 {1,1},
		Vector2 {1,-1}, 

		Vector2 {0,-1}, 
		Vector2 {-1,0}, 
		Vector2 {0,1}, 
		Vector2 {1,0},
	};

	vector<vector<Vector2>> parent = 
	vector<vector<Vector2>>(MAXCOUNT_Y, vector<Vector2>(MAXCOUNT_X, Vector2(-1,-1)));
	vector<vector<int>> best = 
	vector<vector<int>>(MAXCOUNT_Y, vector<int>(MAXCOUNT_X, INT_MAX));
	priority_queue<Vertex_Djikstra, vector<Vertex_Djikstra>, greater<Vertex_Djikstra>> pq;

	Vertex_Djikstra startV;
	startV.pos = start;
	startV.g = 0;
	pq.push(startV);

	best[start._y][start._x] = 0;
	parent[start._y][start._x] = start;

	while (true)
	{
		if(pq.empty())
			break;

		Vertex_Djikstra hereV = pq.top();
		pq.pop();

		if(hereV.pos == _maze->GetEndPos())
			break;

	
		if(best[hereV.pos._y][hereV.pos._x] < hereV.g)
			continue;


		for (int i = 0; i < 8; i++)
		{
			Vector2 there = hereV.pos + frontPos[i];

			if(!Cango(there._y, there._x))
				continue;

			int newCost = 0;
			if(i > 3)
				newCost = hereV.g + 10;
			else
				newCost = hereV.g + 14;

			if(newCost >= best[there._y][there._x])
				continue;

			Vertex_Djikstra thereV;
			thereV.pos = there;
			thereV.g = newCost;

			_maze->SetBlockType(there._y, there._x, Block::BlockType::FOOT_PRINT);

			pq.push(thereV);
			best[there._y][there._x] = newCost;
			parent[there._y][there._x] = hereV.pos;
		}
	}

	Vector2 check = _maze->GetEndPos();
	_path.push_back(check);
	while (true)
	{
		if (check == start) break;

		check = parent[check._y][check._x];
		_path.push_back(check);
	}

	std::reverse(_path.begin(), _path.end());
}

void Player::AStart(Vector2 start, Vector2 end)
{
	Vector2 frontPos[8] =
	{
		Vector2 {-1,-1}, 
		Vector2 {-1,1},
		Vector2 {1,1}, 
		Vector2 {1,-1}, 

		Vector2 {0,-1}, 
		Vector2 {-1,0}, 
		Vector2 {0,1},
		Vector2 {1,0},
	};

	vector<vector<Vector2>> parent =
		vector<vector<Vector2>>(MAXCOUNT_Y, vector<Vector2>(MAXCOUNT_X, Vector2(-1, -1)));
	vector<vector<int>> best =
		vector<vector<int>>(MAXCOUNT_Y, vector<int>(MAXCOUNT_X, INT_MAX));
	priority_queue<Vertex, vector<Vertex>, greater<Vertex>> pq;

	Vertex startV;
	startV.pos = start;
	startV.g = 0;
	startV.h = start.ManhattanDistance(_maze->GetEndPos()) * 10.0f;
	startV.f = startV.g + startV.h;
	pq.push(startV);

	best[start._y][start._x] = startV.f;
	parent[start._y][start._x] = start;

	while (true)
	{
		if (pq.empty())
			break;

		Vertex hereV = pq.top();
		pq.pop();

		if (hereV.pos == _maze->GetEndPos())
			break;

		if (best[hereV.pos._y][hereV.pos._x] < hereV.f)
			continue;

		for (int i = 0; i < 8; i++)
		{
			Vector2 there = hereV.pos + frontPos[i];

			if (!Cango(there._y, there._x))
				continue;

			int newCost = 0;
			if (i > 3)
				newCost = hereV.g + 10;
			else
				newCost = hereV.g + 14;

			int h = there.ManhattanDistance(_maze->GetEndPos()) * 10.0f;
			int f = newCost + h;

			if (f >= best[there._y][there._x])
				continue;

			Vertex thereV;
			thereV.pos = there;
			thereV.g = newCost;
			thereV.h = h;
			thereV.f = f;

			pq.push(thereV);
			_maze->SetBlockType(there._y, there._x, Block::BlockType::FOOT_PRINT);
			best[there._y][there._x] = f;
			parent[there._y][there._x] = hereV.pos;
		}
	}

	Vector2 check = _maze->GetEndPos();
	_path.push_back(check);
	while (true)
	{
		if (check == start) break;

		check = parent[check._y][check._x];
		_path.push_back(check);
	}

	std::reverse(_path.begin(), _path.end());
}

bool Player::Cango(int y, int x)
{
	Block::BlockType blockType = _maze->GetBlockType(y,x);
	if(blockType == Block::BlockType::ABLE)
		return true;

	return false;
}

void Player::Update()
{
	if (_pathIndex >= _path.size())
	{
		_pathIndex = 0;
		_path.clear();

		_maze->CreateMaze_Kruskal();
		BeginPlay();

		return ;
	}

	_time += 0.5f;
	if (_time > 1.0f)
	{
		_time = 0.0f;
		_pos = _path[_pathIndex];

		if (_pathIndex != 0)
		{
			_maze->SetPlayerPos(_pos);
		}

		_pathIndex++;
	}
}
