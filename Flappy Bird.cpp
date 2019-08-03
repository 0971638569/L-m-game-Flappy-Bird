#include<iostream>
#include<CoreWindow.h>
#include<ctime>
#include <list>

using namespace std;
enum Action { ESC, FIRE, NONE,};
enum WalkDirection { UP, DOWN };


class KeyBoard
{
public:
	static Action GetAction()
	{
		Sleep(10);
		if (GetAsyncKeyState(32))
		{
			return Action::FIRE;
		}
		if (GetAsyncKeyState(27))
		{
			return Action::ESC;
		}

		return Action::NONE;
	}
};
class Cursor
{
	HANDLE handle;
public:
	static Cursor Dedault;
public:
	Cursor(HANDLE handle = 0)
	{
		if (handle == 0)
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
		this->handle = handle;
	}

	void Show()
	{
		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(handle, &info);

		info.bVisible = true;
		SetConsoleCursorInfo(handle, &info);
	}

	void Hide()
	{
		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(handle, &info);

		info.bVisible = false;
		SetConsoleCursorInfo(handle, &info);
	}

	void MoveTo(int x, int y)
	{
		COORD coord;
		coord.X = x;
		coord.Y = y;

		SetConsoleCursorPosition(handle, coord);
	}
};
class Size
{
public:
	int width, height;
public:
	Size() { width = height = 0; }
	Size(int w, int h) : width(w), height(h) {}
	friend Size operator+(const Size &left, const Size &right)
	{
		return Size(left.width + right.width, left.height + right.height);
	}
	friend ostream& operator<<(ostream& left, const Size& right)
	{
		return left << "{ " << right.width << ", " << right.height << " }";
	}
};
class Point
{
public:
	int x, y;
public:
	Point() { x = y = 0; }
	Point(int x, int y) : x(x), y(y) {}

	friend Point operator+(const Point &left, const Size &right)
	{
		return Point(left.x + right.width, left.y + right.height);
	}
	friend ostream& operator<<(ostream& left, const Point& right)
	{
		return left << "{ " << right.x << ", " << right.y << " }";
	}

};
class Rectangle
{
public:
	Point location;	// Toa tren ben trai
	Size size;	// Kich thuoc
public:
	Rectangle() {}
	Rectangle (int x, int y, int w, int h)
		: location(x, y)
		, size(w, h)
	{
	}
public:
	Point BottomRight() const { return location + size; }

};
class Frame: public Rectangle
{
public:
	Cursor cursor;

public:
	Frame() { }
	Frame(int x, int y, int w, int h)
		: Rectangle(x, y, w, h)
	{
	}
public:
	void DrawLine(Point &p, int left, int middle, int right)
	{
		cursor.MoveTo(p.x, p.y++);
		cout << (char)left;
		for (int i = 2; i < size.width; i++)
			cout << (char)middle;
		cout << (char)right;
	}
	void Draw()
	{
		Point p = location;
		DrawLine(p, 201, 205, 187);
		for (int i = 2; i < size.height; i++)
			this->DrawLine(p, 186, 32, 186);
		DrawLine(p, 200, 205, 188);
		cursor.Hide();
	}
};
class Plane : public Point
{
public:
	int xP, yP;
	Frame board;
public:
	Plane()
	{
		xP = yP = 0;
	}
	Plane& operator=(const Point &p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}
	void MovePlane()
	{
		xP = board.location.x + x * 3;
		yP = board.location.y + y + 1;
		board.cursor.MoveTo (xP, yP);
	}
	void ShowPlane()
	{
		MovePlane ();
		cout << char(2);
	}
	char GetValue()
	{
		return ' ';
	}

	void Clear()
	{
		MovePlane();
		cout << GetValue();
	}

	void Move1(int x, int y)
	{
		Clear();
		x += x;
	}
};
class Walk : public Point
{	
public:
	int height;
	WalkDirection direction;
	Frame board;

public:
	Walk& operator=(const Point &p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}
	Walk(WalkDirection dir, Frame frame)
	{
		board = frame;
		x = board.size.width - 2;
		int max = (board.size.height - 1) / 2;
		height = rand() % (max - 2 + 1 ) + 1;

		direction = dir;
	}
	void MoveWalk()
	{
		Show(32);
		--x;
		Show(176);
	}
	void Show(int c)
	{
		Cursor cursor;

		int y = direction == UP ? 1 : board.size.height - height - 1;
		
		this->y = y;
		for (int i = 0; i < height; i++)
		{
			int _x = board.location.x + x;
			int _y = board.location.y + y + i;
			cursor.MoveTo(_x, _y);
			cout << (char)c;
		}
	}
	int isEnd()
	{
		return x < 2;
	}


};
class Score
{
public:
	int score;
	
	Frame board;
public:
	Score()
	{
		score = 0;
		
	}
	void Show()
	{
		int _x = board.size.width + 4;
		int _y = board.size.height / 2;
		board.cursor.MoveTo(_x,_y);
		cout << "Score: " << score << "  ";
	}
	
};
class HightScore
{
public:
	int score;
	Frame board;
public:
	HightScore()
	{
		score = 0;
	}
	void Show()
	{
		int _x = board.size.width + 4;
		int _y = board.size.height / 2 - 1;
		board.cursor.MoveTo(_x,_y);
		cout << "HightScore: " << score;
	}
};
class Start
{
public:

	int menu;
	Frame board;
public:
	Start()
	{
		menu = 0;
	}
	void Show()
	{
		int _x = board.size.width / 2;
		int _y = board.size.height / 2;
		board.cursor.MoveTo(_x,_y);
		cout <<"1. ReStart ";

	}
	void clear()
	{
		cout << " ";
	}
};
class Exit
{
public:

	int menu;
	Frame board;
public:
	Exit()
	{
		menu = 0;
	}
	void Show()
	{
		int _x = board.size.width / 2;
		int _y = board.size.height / 2 + 1;
		board.cursor.MoveTo(_x,_y);
		cout <<"2. Exit \n\n\n\n\n\n\n\n\n\n\n";
	}
	void clear()
	{
		cout << " ";
	}
};

class Game
{
	Frame board;
	int size;
	int speed;
	int diem;
	Plane plane;
	Score score;
	Start menu;
	Exit exit;
	HightScore  hscore;
public:
	Game(int size)
		: size(size)
		, board(1,1,size * 3 + 2,size + 2)
	{
		board.Draw();
		plane.board = board;
		plane = Point(size / 4, size);
		
		score.board = board ;
		score.Show ();
		hscore.board = board ;
		hscore.Show ();

		plane.x = (plane.x + size) % size;
		plane.y = (plane.y + size) % size;
		plane.ShowPlane();

	}
	void playgame()
	{ 
back:

		list<Walk> walks;
		speed = 11;
		int count = speed - 1;
		int count2 = 0;
		bool check = false;
		while(1)
		{ 
			Sleep(100);
			plane .Clear();

			if (++count ==speed)
			{
				count2 += 1;
				walks.push_back(Walk(UP, board));
				walks.push_back(Walk(DOWN, board));
				count = 0;
			}
			if(62 - count2  == 57)
			{
				score.score += 1;
				score.Show ();
				count2 = 4;
				if(hscore .score < score .score )
				{
					hscore .score = score.score ;
					hscore .Show ();
				}
			}
			if(walks.front ().isEnd())
			{
				walks.front().Show (32);
				walks.pop_front ();
				walks.front().Show (32);
				walks.pop_front ();
			}
			for (auto &w : walks)
			{
				w.MoveWalk();
				if ((w.x) == plane.xP)
				{
					if (w.direction == UP)
					{
						if (plane.yP > w.y && plane.yP <= w.height)
						{
							check = true;
						}
					}else
					{
						if (plane.yP >= w.y && plane.yP < (w.y + w.height))
						{
							check = true;
						}
					}
				}
			}
			if (check)
			{
				plane.ShowPlane();
				menu.board = board ;
				menu.Show ();
				exit.board = board ;
				exit.Show ();
				int a; cin >> a;
				switch(a)
				{
				case 1:
					board.Draw();
					plane.Clear();
					plane .y = 1;
					score.score = 0;
					score.Show();
					goto back;
					break;
				default: return; break;
				}
			}

			plane.y++; 
			plane.ShowPlane();

			if(plane.y == size - 1)
			{
				menu.board = board ;
				menu.Show ();
				exit.board = board ;
				exit.Show ();
				int a; cin >> a;
				switch(a)
				{
				case 1:
					board.Draw();
					plane.Clear();
					Showgame(0,-19);
					score.score = 0;
					score.Show();
					goto back;
					break;
				default: return; break;
				}

			}

			if(plane.y == size - ( size - 1))
				continue ;

			Action a = KeyBoard :: GetAction ();
			switch (a)
			{
			case FIRE :
				Showgame(0, -2); 
				continue;
			case ESC :
				return;
			}
		}

	}

	void Showgame(int x, int y)
	{
		plane.Clear();
		plane.x = (plane.x + x + size) % size;
		plane.y = (plane.y + y + size) % size;
		plane.ShowPlane();
	}

};


int main()
{
	Game game(20);
	game.playgame();
	system("pause");
	return 0 ;
}; 