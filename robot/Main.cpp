#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct RobotCoordinate
{
	int x, y;
};

bool isInValidIndexInTable(RobotCoordinate coordinate, int** table, int rows, int cols)
{
	int x = coordinate.x;
	int y = coordinate.y;

	return x < 0 || y < 0 || x >= rows || y >= cols || table[x][y] == -1;
}

RobotCoordinate* getNextMove(int** table, int rows, int cols, RobotCoordinate& currentCoordinate)
{
	int x = currentCoordinate.x;
	int y = currentCoordinate.y;


	RobotCoordinate* nextMove = nullptr;
	try
	{
		nextMove = new RobotCoordinate{ x, y + 1 };
	}
	catch (const std::bad_alloc&)
	{
		return nullptr;
	}

	int dx[] = { 0, 1, -1 };
	int dy[] = { -1, 0, 0 };

	for (int i = 0; i < 3; i++)
	{
		int newX = x + dx[i];
		int newY = y + dy[i];
		if (!isInValidIndexInTable({ newX, newY }, table, rows, cols) && table[newX][newY] > table[nextMove->x][nextMove->y])
		{
			nextMove->x = newX;
			nextMove->y = newY;
		}
	}

	if (isInValidIndexInTable(*nextMove, table, rows, cols))
	{
		delete nextMove;
		return nullptr;
	}

	return nextMove;
}

void setTableFromFile(int** table, int rows, int cols, ifstream& inputFile)
{
	for (int i = 0; i < rows; i++)
	{
		table[i] = new int[cols];
		for (int j = 0; j < cols; j++)
		{
			int value;
			inputFile >> value;
			table[i][j] = value;
		}
	}
}

int moveSingleRobotAndReturnTotalPoint(int** table, int rows, int cols, ofstream& outputFile, vector<RobotCoordinate>& path)
{
	int startX, startY;

	cout << "Nhap vao toa do cua robot: ";
	cin >> startX >> startY;

	RobotCoordinate* newCoordinate = new RobotCoordinate{ startX,startY };

	int sum = table[startX][startY];
	table[startX][startY] = -1;
	path.push_back({ startX, startY });

	while (true)
	{
		newCoordinate = getNextMove(table, rows, cols, *newCoordinate);
		if (newCoordinate == nullptr)
			break;
		path.push_back({ newCoordinate->x, newCoordinate->y });
		outputFile << table[newCoordinate->x][newCoordinate->y] << "\t";
		sum += table[newCoordinate->x][newCoordinate->y];
		table[newCoordinate->x][newCoordinate->y] = -1;
	}
	outputFile << endl;
	for (const auto& p : path)
	{
		cout << "(" << p.x << ", " << p.y << ") ";
	}
	cout << endl;

	outputFile << sum << endl;
	return sum;
}
void moveTwoRobotIndependentAndCompareThem(int** table, int rows, int cols, ofstream& outputFile)
{
	int** table2 = new int* [rows];
	for (int i = 0; i < rows; i++) {
		table2[i] = new int[cols];
		memcpy(table2[i], table[i], cols * sizeof(int));
	}

	vector<RobotCoordinate> path1;
	int sum1 = moveSingleRobotAndReturnTotalPoint(table, rows, cols, outputFile, path1);

	vector<RobotCoordinate> path2;
	int sum2 = moveSingleRobotAndReturnTotalPoint(table2, rows, cols, outputFile, path2);

	cout << "Duong di cua robot 1: " << endl;
	outputFile << "Duong di cua robot 1: " << endl;
	for (const auto& p : path1)
	{
		cout << "(" << p.x << ", " << p.y << ") ";
		outputFile << "(" << p.x << ", " << p.y << ") ";
	}
	cout << endl;
	outputFile << endl;
	cout << "Tong gia tri da di duoc cua robot 1: " << sum1 << endl;
	outputFile << "Tong gia tri da di duoc cua robot 1: " << sum1 << endl;

	cout << "Duong di cua robot 2: " << endl;
	outputFile << "Duong di cua robot 2: " << endl;
	outputFile << endl;
	for (const auto& p : path2)
	{
		cout << "(" << p.x << ", " << p.y << ") ";
		outputFile << "(" << p.x << ", " << p.y << ") ";
	}
	cout << endl;
	cout << "Tong gia tri da di duoc cua robot 2: " << sum2 << endl;
	outputFile << "Tong gia tri da di duoc cua robot 2: " << sum2 << endl;


	string messageCompare = (sum1 > sum2) ? "Robot 1 co diem cao hon robot 2"
		: (sum1 == sum2 ? "Robot 1 co diem bang robot 2" : "Robot 1 co diem thap hon robot 2");

	outputFile << messageCompare << endl;
	cout << messageCompare << endl;

	vector<RobotCoordinate> commonCoordinates;
	for (const auto& p1 : path1)
	{
		for (const auto& p2 : path2)
		{
			if (p1.x == p2.x && p1.y == p2.y)
			{
				commonCoordinates.push_back(p1);
				break;
			}
		}
	}

	cout << "Toa do: " << endl;

	for (const auto& p : commonCoordinates)
	{
		cout << "(" << p.x << ", " << p.y << ") ";
	}
	cout << endl;

	outputFile << "Toa do: " << endl;
	for (const auto& p : commonCoordinates)
	{
		outputFile << "(" << p.x << ", " << p.y << ") ";
	}
	outputFile << endl;
}
void moveTwoRobotTurnBase(int** table, int rows, int cols, ofstream& outputFile)
{
	int startX1, startY1, startX2, startY2;
	vector<RobotCoordinate> path1, path2;
	bool isRobot1Finished = false;
	bool isRobot2Finished = false;
	int** table2 = new int* [rows];

	for (int i = 0; i < rows; i++) {
		table2[i] = new int[cols];
		memcpy(table2[i], table[i], cols * sizeof(int));
	}

	cout << "Nhap vao toa do diem bat dau cua robot 1: ";
	cin >> startX1 >> startY1;
	cout << "Nhap vao toa do diem bat dau cua robot 2: ";
	cin >> startX2 >> startY2;

	int sum1 = table[startX1][startY1];
	int sum2 = table[startX1][startY1];

	path1.push_back({ startX1, startY1 });
	path1.push_back({ startX2, startY2 });
	table[startX1][startY1] = -1;
	table[startX2][startY2] = -1;
	RobotCoordinate* newCoordinate1 = new RobotCoordinate{ startX1,startY1 };
	RobotCoordinate* newCoordinate2 = new RobotCoordinate{ startX2,startY2 };

	while (true)
	{
		if (!isRobot1Finished) {
			newCoordinate1 = getNextMove(table, rows, cols, *newCoordinate1);
			if (newCoordinate1 == nullptr) {
				isRobot1Finished = true;
			}
			else {
				path1.push_back({ newCoordinate1->x, newCoordinate1->y });
				outputFile << "robot1:" << table[newCoordinate1->x][newCoordinate1->y] << "\t";
				sum1 += table[newCoordinate1->x][newCoordinate1->y];
				table[newCoordinate1->x][newCoordinate1->y] = -1;
			}
		}
		if (!isRobot2Finished) {
			newCoordinate2 = getNextMove(table, rows, cols, *newCoordinate2);
			if (newCoordinate2 == nullptr) {
				isRobot2Finished = true;
			}
			else {
				path2.push_back({ newCoordinate2->x, newCoordinate2->y });
				outputFile << "robot2:" << table[newCoordinate2->x][newCoordinate2->y] << "\t";
				sum2 += table[newCoordinate2->x][newCoordinate2->y];
				table[newCoordinate2->x][newCoordinate2->y] = -1;
			}
		}
		if (isRobot1Finished && isRobot2Finished) {
			break;
		}
	}
	cout << "Duong di cua robot 1: " << endl;
	outputFile << "Duong di cua robot 1: " << endl;
	for (const auto& p : path1)
	{
		cout << "(" << p.x << ", " << p.y << ") ";
		outputFile << "(" << p.x << ", " << p.y << ") ";
	}
	cout << endl;
	outputFile << endl;
	cout << "Tong gia tri da di duoc cua robot 1: " << sum1 << endl;
	outputFile << "Tong gia tri da di duoc cua robot 1: " << sum1 << endl;

	cout << "Duong di cua robot 2: " << endl;
	outputFile << "Duong di cua robot 2: " << endl;
	outputFile << endl;
	for (const auto& p : path2)
	{
		cout << "(" << p.x << ", " << p.y << ") ";
		outputFile << "(" << p.x << ", " << p.y << ") ";
	}
	cout << endl;
	cout << "Tong gia tri da di duoc cua robot 2: " << sum2 << endl;
	outputFile << "Tong gia tri da di duoc cua robot 2: " << sum2 << endl;
	string messageCompare = (sum1 > sum2) ? "Robot 1 co diem cao hon robot 2"
		: (sum1 == sum2 ? "Robot 1 co diem bang robot 2" : "Robot 1 co diem thap hon robot 2");

	outputFile << messageCompare << endl;
	cout << messageCompare << endl;
}
void printTable(int** table, int rows, int cols) {
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout << table[i][j] << '\t';
		}
		std::cout << '\n';
	}
}
int main()
{
	int option;
	cout << "Menu: " << endl;
	cout << "1. Chay mot robot" << endl;
	cout << "2. Chay 2 robot doc lap" << endl;
	cout << "3. Chay 2 robot theo luot" << endl;
	cout << "Enter your option: ";
	cin >> option;

	ifstream inputFile("input.txt");
	int rows, cols;
	int** table;

	inputFile >> rows >> cols;
	table = new int* [rows];
	setTableFromFile(table, rows, cols, inputFile);
	printTable(table, rows, cols);
	switch (option)
	{
	case 1: {
		ofstream outputFile("output.txt", ofstream::trunc);
		vector<RobotCoordinate> path;

		moveSingleRobotAndReturnTotalPoint(table, rows, cols, outputFile, path);
		outputFile.close();
		break;
	}
	case 2: {
		ofstream outputFile("output2.txt", ofstream::trunc);

		moveTwoRobotIndependentAndCompareThem(table, rows, cols, outputFile);
		outputFile.close();
		break;
	}
	case 3:
		ofstream outputFile("output3.txt", ofstream::trunc);

		moveTwoRobotTurnBase(table, rows, cols, outputFile);
		outputFile.close();
		break;
	}
	for (int i = 0; i < rows; i++)
	{
		delete[] table[i];
	}
	delete[] table;

	inputFile.close();

	return 0;
}
