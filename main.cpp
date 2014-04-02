#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include<vector>

using namespace std;
int f_tranzitie[100][100], stari, stari_finale[100], stare_initiala, nr_stari_finale,stari_viz[100];
char alfabet[50];
int relatii[100][100];
queue<int> que_stari;
void read()
{
	ifstream f("automat.txt");
	f >> stari;
	f >> stare_initiala;
	f >> nr_stari_finale;
	for (int i = 0; i < nr_stari_finale; i++)
		f >> stari_finale[i];
	f >> alfabet;
	for (int i = 0; i < stari; i++)
	for (int j = 0; j < strlen(alfabet); j++)
		f >> f_tranzitie[i][j];

}
int indice_litera(char l)
{
	for (int i = 0; i < strlen(alfabet);i++)
	if (l == alfabet[i])
		return i;
	return -1;
}
char * valid_word(char * cuvant)
{
	int i = stare_initiala;
	int ind_lit;
	for (int k = 0; k < strlen(cuvant);k++)
	{
		ind_lit = indice_litera(cuvant[k]);
		if (ind_lit == -1)
			return "Nu";

		i = f_tranzitie[i][ind_lit];

		if (i == -1)
			return "Nu";
	}
	for (int k = 0; k < nr_stari_finale;k++)
	if (i == stari_finale[k])
		return "Da";

	return "Nu";
}

void scrie_automat()
{	
	cout << "   ";
	for (int j = 0; j < strlen(alfabet); j++)
		cout << alfabet[j] << " ";
	cout << endl;
	for (int i = 0; i < stari; i++)
	{
		if (f_tranzitie[i][99] != 1){
			cout << i << ": ";
			for (int j = 0; j < strlen(alfabet); j++)
				cout << f_tranzitie[i][j] << " ";
			cout << endl;
		}
	}
}
void stergeLinie(int i)
{
	for (int j = 0; j < strlen(alfabet); j++)
	for (int k = i; k < stari - 1; k++)
	{
		f_tranzitie[k][j] = f_tranzitie[k + 1][j];

	}
	for (int k = i; k < stari - 1; k++)
		stari_viz[k] = stari_viz[k + 1];
	for (int j = 0; j < strlen(alfabet); j++)
	for (int k = 0; k < stari; k++)
	{
		if (f_tranzitie[k][j]>i)f_tranzitie[k][j]--;
	}

	for (int j = 0; j < nr_stari_finale; j++)
	if (stari_finale[j]>i)stari_finale[i]--;
	stari--;
	i--;
}
void eliminare_stari_inaccesibile()
{
	for (int i = 0; i < stari; i++)
		stari_viz[i] = 0;
	stari_viz[stare_initiala] = 1;
	que_stari.push(stare_initiala);
	int stare_act;
	while (!que_stari.empty())
	{
		stare_act = que_stari.front();
		que_stari.pop();
		for (int i = 0; i < strlen(alfabet); i++)
		{
			if (stari_viz[f_tranzitie[stare_act][i]] == 0)
			{
				que_stari.push(f_tranzitie[stare_act][i]);
				stari_viz[f_tranzitie[stare_act][i]] = 1;
			}
		}
	}
	for (int i = 0; i < stari; i++)
	{
		if (stari_viz[i] == 0)
		{
			/*
			for (int k = 0; k < stari; k++)
				f_tranzitie[i][k] = -2;*/
			stergeLinie(i);
		}
	}
}
int checkStareFinala(int k)
{
	for (int i = 0; i < nr_stari_finale;i++)
	if (k == stari_finale[i])
		return 1;
	return 0;
}
void scrie_rel()
{
	for (int i = 1; i < stari; i++)
	{
		cout << i << ": ";
		for (int j = 0; j <i; j++)
			cout << relatii[i][j] << " ";
		cout << endl;
	}
	cout << "----------------------------------------------------\n";
}
void construieste_relatii_echiv()
{
	eliminare_stari_inaccesibile();
	int nr_stari_nefinale = 0;
	int stari_nefinale[100];
	for (int i = 0; i < stari; i++)
	if (!checkStareFinala(i))
	{
		stari_nefinale[nr_stari_nefinale] = i;
		nr_stari_nefinale++;
	}

	for (int i = 0; i < nr_stari_finale; i++)
	{
		for (int k = 0; k < nr_stari_nefinale; k++)
			relatii[stari_finale[i]][stari_nefinale[k]] = relatii[stari_nefinale[k]][stari_finale[i]] = 1;
	}
	int sem = 1;
	int c = 1;
	while (sem)
	{
		c++;
		scrie_rel();
		sem = 0;
		for (int i = 0; i < stari;i++)
		for (int j = 0; j < i; j++)
		{
			for (int l = 0; l < strlen(alfabet);l++)
			{
				if (relatii[f_tranzitie[i][l]][f_tranzitie[j][l]] !=0 && relatii[i][j]==0)
				{
					relatii[i][j] = relatii[j][i] = c;
					sem = 1;
				}
			}
		}
		
	}
	int stari_finale_noi[100];
	int nr_stari_finale_nou = stari;
	int f_tranz_nou[100][100];
	vector<pair<int,int>> temp;
	int stariechiv[100][100];
	for (int i = 0; i < stari; i++)
	{
		int x, y;
		x = y = i;
		
		for (int j = 0; j < i;j++)
		if (relatii[i][j] == 0)
			y = j;
		temp.push_back(make_pair(x,y));
	}
/*	for (int i = 0; i < temp.size(); i++)
		cout << temp[i].first << " " << temp[i].second << endl;
	cout << endl;*/
	for (int i = 0; i < nr_stari_finale; i++)
	{
		for (int j = 0; j < temp.size();j++)
		if (temp[j].first ==stari_finale[i])
		{
			int m = j;
			while (temp[m].first != temp[m].second)
				m = temp[m].second;
			stari_finale_noi[nr_stari_finale_nou++] = temp[m].second;
		}
	}
	for (int i = 0; i < stari; i++)
	{
		
		for (int j = 0; j < strlen(alfabet); j++)
		{
			int x = f_tranzitie[i][j];
			for (int k = 0; k < temp.size();k++)
			if (temp[k].first == x)
			{
				int m = k;
				while (temp[m].first != temp[m].second)
					m = temp[m].second;
				x = temp[m].second;
			}
			f_tranz_nou[i][j] = x;
			//cout << x << " ";
				
		}
		//cout << endl;
	}
	for (int i = 0; i < temp.size();i++)
	if (temp[i].first != temp[i].second)
	{
		f_tranz_nou[temp[i].first][99] = 1;
		/*stergeLinie(i);
		for (int j = 0; j < temp.size(); j++)
		{
			if (i<temp[j].first)temp[j].first--;
			if (i<temp[j].second)temp[j].second--;
		}*/
	}
	cout << "   ";
	for (int j = 0; j < strlen(alfabet); j++)
		cout << alfabet[j] << " ";
	cout << endl;
	for (int i = 0; i < stari; i++)
	{
		f_tranzitie[i][99] = f_tranz_nou[i][99];
		if (f_tranz_nou[i][99]!=1){
			cout << i << ": ";
			for (int j = 0; j < strlen(alfabet); j++)
			{
				f_tranzitie[i][j] = f_tranz_nou[i][j];
				cout << f_tranz_nou[i][j] << " ";
			}
			cout << endl;
		}
	}
	

}
int main()
{
	read();
	char cuvant[100];
	cin >> cuvant;
	while (strcmp(cuvant, "exit") != 0)
	{
		if (strcmp(cuvant, "scrie")==0)scrie_automat();
		if (strcmp(cuvant, "min") == 0)construieste_relatii_echiv();

			cout << cuvant << " - " << valid_word(cuvant) << '\n';
			cin >> cuvant;
		
	}

	cout << endl;
	system("pause");
	return 0;
}
