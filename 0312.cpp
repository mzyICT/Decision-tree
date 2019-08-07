#include"stdafx.h"
#include <random>
#include <stdlib.h>
#include<cmath>
#include <time.h>
using namespace std;
enum Outlook {Sunny=1,Overcast=2,Rain=3};
enum Temperature {Hot=1,Mild=2,Cool=3};
enum Humidity {Normal=1,High=2};
enum Wind {Weak=1,Strong=2};
enum State{yes=1,no=2};

Outlook rand_outlook()
{
	srand((unsigned)time(NULL));
	int n = 3;
	double t, p1, p2, p3;
	Outlook get;
	p1 = 5. / 14;
	p2 = 4. / 14;
	p3 = 5. / 14;
	//随机生成一个n（如100）以内的数， 
	t = rand() % n + 1;
	if (t < n *p1)
	{
		get = Sunny;
		return get;
		//拿到1
	}
	else if (t < n*(p1 + p2))
	{
		get = Overcast;
		return get;
		//拿到2
	}
	else if (t<n*(p1 + p2 + p3))
	{
		get = Rain;
		return get;
		//拿到3
	}
	else
	{
		get = Rain;
		return get;
	}
}

Temperature rand_tem()
{
	srand((unsigned)time(NULL));
	int n = 3;
	double t, p1, p2, p3;
	Outlook before = rand_outlook();
	Temperature get;
	if (before == Sunny)
	{
		p1 = .4;
		p2 = .4;
		p3 = .2;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = Hot;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = Mild;
			return get;
		}
		else if (t<n*(p1 + p2 + p3))
		{
			get = Cool;
			return get;
		}
		else
		{
			get = Cool;
			return get;
		}

	}

	else if (before == Overcast)
	{
		p1 = .5;
		p2 = .25;
		p3 = .25;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = Hot;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = Mild;
			return get;
		}
		else if (t<n*(p1 + p2 + p3))
		{
			get = Cool;
			return get;
		}
		else
		{
			get = Cool;
			return get;
		}

	}

	else
	{
		p1 = 0;
		p2 = .6;
		p3 = .4;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = Hot;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = Mild;
			return get;
		}
		else if (t<n*(p1 + p2 + p3))
		{
			get = Cool;
			return get;
		}
		else
		{
			get = Cool;
			return get;
		}
	}
	
}


Humidity rand_hum()
{
	srand((unsigned)time(NULL));
	int n = 2;
	double t, p1, p2;
	Temperature before = rand_tem();
	Humidity get;
	if (before == Hot)
	{
		p1 = .25;
		p2 = .75;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = High;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = Normal;
			return get;
		}
		else
		{
			get = Normal;
			return get;
		}

	}

	else if (before == Mild)
	{
		p1 = 1./3;
		p2 = 2./3;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = High;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = Normal;
			return get;
		}

		else
		{
			get = Normal;
			return get;
		}

	}

	else
	{
		p1 = 1;
		p2 = 0;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = High;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = Normal;
			return get;
		}
		else
		{
			get = Normal;
			return get;
		}
	}
}


Wind rand_wind()
{
	srand((unsigned)time(NULL));
	int n = 2;
	double t, p1, p2;
	Humidity before = rand_hum();
	Wind get;
	if (before == Normal)
	{
		p1 = 0.57142857;
		p2 = 0.42857143;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = Weak;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = Strong;
			return get;
		}
		else
		{
			get = Strong;
			return get;
		}

	}

	else
	{
		p1 = 0.57142857;
		p2 = 0.42857143;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = Weak;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = Strong;
			return get;
		}
		else
		{
			get = Strong;
			return get;
		}
	}
}


State rand_sta()
{
	srand((unsigned)time(NULL));
	int n = 2;
	double t, p1, p2;
	Wind before = rand_wind();
	State get;
	if (before == Weak)
	{
		p1 = 2. / 3;
		p2 = 1. / 3;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = yes;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = no;
			return get;
		}
		else
		{
			get = no;
			return get;
		}

	}

	else
	{
		p1 = 2. / 3;
		p2 = 1. / 3;
		t = rand() % n + 1;
		if (t < n *p1)
		{
			get = yes;
			return get;
		}
		else if (t < n*(p1 + p2))
		{
			get = no;
			return get;
		}
		else
		{
			get = no;
			return get;
		}
	}
}

int main()
{
		Outlook get_Outlook = rand_outlook();
		Temperature get_Temperature = rand_tem();
		Humidity get_Humidity = rand_hum();
		Wind get_Wind = rand_wind();
		State get_State = rand_sta();
		return 0;
}

