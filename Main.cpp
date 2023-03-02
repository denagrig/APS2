#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <conio.h>
#include <limits.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "server.h"
#include "buffer.h"
#include "distributor.h"

#pragma warning(disable : 4996)

static Statistic globalStat(0, 0, 0, 0, 0.0, 0.0, 0.0);
static int step_;
static bool next;
static bool back;

int requestAmount_;
int userAmount_;
int bufferSize_;
int execAmount_;
double lambda_;
double alpha_;
double beta_;

const int W = 602;
const int H = 602;

using namespace sf;

void run(bool mode);

void table(std::vector<std::vector<std::string>>& data, int width, int hight);

void fillDataStat(std::vector<std::vector<std::string>>& data);

void fillDataStatForGraph(std::vector<std::vector<float>>& data);

void fillDataExecUsage(std::vector<std::vector<std::string>>& data);

void stepMode();

int drawGraph(int userCount, std::vector<std::vector<float>> data);

int main(int argc, char* argv[])
{
  std::cout << "Choose working mode: 0 - table (test), 1 - step, 2 - graphics 3 - find optimal value : ";
  int choose;
  std::cin >> choose;
  if (choose == 0)
  {
    run(false);
    std::vector<std::vector<std::string>> data(globalStat.getUserAmount() + 1, std::vector<std::string>(8));
    fillDataStat(data);
    table(data, 8, globalStat.getUserAmount() + 1);

    std::cout << "\n\n\n";

    std::vector<std::vector<std::string>> data1(globalStat.getExecAmount() + 1, std::vector<std::string>(2));
    fillDataExecUsage(data1);
    table(data1, 2, globalStat.getExecAmount() + 1);
  }
  else if (choose == 1)
  {
    run(false);
    step_ = 0;
    next = true;
    back = false;
    bool statusMode = true;
    bool statusStep = false;
    while (statusMode)
    {
      statusStep = false;
      system("cls");
      std::cout << "\n     " << char(17) << "    : step back\n     " << char(16)
        << "    : step forward\n  [space] : jump to X step\n    "
        << char(30) << " " << char(31) << "   : exit\n"
        << "_______________________________";
      stepMode();
      while (!statusStep)
      {
        int button = getch();
        if (button == 224 || button == 0)
        {
          int button = getch();
          switch (button)
          {
          case 75:
            if (back)
            {
              step_--;
              statusStep = true;
            }
            break;
          case 77:
            if (next)
            {
              step_++;
              statusStep = true;
            }
            break;
          default:
            statusMode = false;
            statusStep = true;
          }
        }
        else if (button == 32)
        {
          std::size_t st = 0;
          std::cout << "\n\n Jump to ";
          std::cin >> st;
          if (!std::cin)
          {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<int>::max(), '\n');
            printf("\033[A\r\033[K\033[A\033[A");
            continue;
          }
          step_ = (st > globalStat.getStepVector().size()) ? globalStat.getStepVector().size() - 1 : st - 1;
          step_ = (step_ < 0) ? 0 : step_;
          statusStep = true;
        }
      }
    }
  }
  else if (choose == 2)
  {
    run(false);
    std::vector<std::vector<float>> data(globalStat.getUserAmount(), std::vector<float>(7));
    fillDataStatForGraph(data);

    drawGraph(data.size(), data);
    /*std::vector<std::vector<std::string>> data1(globalStat.getExecAmount() + 1, std::vector<std::string>(2));
    fillDataExecUsage(data1);
    table(data1, 2, globalStat.getExecAmount() + 1);*/
  }
  else
  {
    int buffer = 10;
    int exec = 15;
    requestAmount_ = 1000;
    userAmount_ = 10;
   // alpha_ = 0.5;
    //beta_ = 0.6;
    lambda_ = 0.2;
    for (alpha_ = 0.1; alpha_ < 1; alpha_ += 0.1)
    {
      beta_ = alpha_ + 0.1;
      std::cout << "\nalpha = " << alpha_ << ", beta = " << beta_ << "\n__________________________";
      for (int i = 1; i <= buffer; i++)
      {
        for (int j = 1; j <= exec; j++)
        {
          bufferSize_ = i;
          execAmount_ = j;
          run(true);
          double rejProb = 0.0;
          double usingProb = 0.0;
          for (int k = 0; k < i; k++)
          {
            rejProb += globalStat.getRejectProbality(k);
          }
          rejProb /= i;
          for (int k = 0; k < j; k++)
          {
            usingProb += globalStat.getExecUsage(k);
          }
          usingProb /= j;
          if (abs(rejProb - 0.1) <= 0.07 && abs(usingProb - 0.9) < 0.07)
          {
            std::cout << "\nOptimal value: buffer size = " << i << ", executors count = " << j << "\nRej. prob. = " << rejProb << ", Using prob. = " << usingProb << "\nPrice = " << i * 10000 + j * (1000 / alpha_);
          }
          //else
          //{
          //  //std::cout << "\n" << abs(rejProb - 0.1) << "  " << abs(usingProb - 0.9);
          //  //std::cout << "\n" << rejProb << "  " << usingProb;
          //}
        }
      }
      std::cout << "\n__________________________\n";
    }
  }
  return 0;
}

void run(bool mode)
{
  int requestAmount;
  int userAmount;
  int bufferSize;
  int execAmount;
  double lambda;
  double alpha;
  double beta;
  if (!mode)
  {
    std::cout << "Enter start values:\n";
    std::cout << "  Request amount: ";
    std::cin >> requestAmount;
    std::cout << "  User amount: ";
    std::cin >> userAmount;
    std::cout << "  Buffer size: ";
    std::cin >> bufferSize;
    std::cout << "  Count of executers: ";
    std::cin >> execAmount;
    std::cout << "  Lambda: ";
    std::cin >> lambda;
    std::cout << "  Alpha: ";
    std::cin >> alpha;
    std::cout << "  Beta: ";
    std::cin >> beta;
  }
  else
  {
    requestAmount = requestAmount_;
    userAmount = userAmount_;
    bufferSize = bufferSize_;
    execAmount = execAmount_;
    lambda = lambda_;
    alpha = alpha_;
    beta = beta_;
  }

  Statistic statistic(requestAmount, userAmount, bufferSize, execAmount, lambda, alpha, beta);

  Server server(statistic);
  Buffer buffer(statistic);
  Distributor distributor(statistic);

  int requestNumber = 0;
  bool isRejected = false;
  step step;
  step.userStateVector = std::vector<requestPointer>(statistic.getUserAmount(), nullptr);
  step.bufferStateVector = std::vector<requestPointer>(statistic.getBufferSize(), nullptr);
  step.execStateVector = std::vector<requestPointer>(statistic.getExecAmount(), nullptr);
  step.stepNumber = 0;

  while (statistic.getProcessed() + statistic.getRejected() != statistic.getRequestAmount())
  {
    if (requestNumber != statistic.getRequestAmount())
    {
      server.getRequest(requestNumber)->setSerialNumber(requestNumber + 1);
      requestPointer req = server.getRequest(requestNumber, step);
      statistic.addStep(step);
      statistic.addUserTime(step.time);
      statistic.addUserStep(step);

      for (int i = 0; i < step.userStateVector.size(); i++)
      {
        step.userStateVector[i] = nullptr;
      }
      isRejected = buffer.addRequest(req, step);
      if (!isRejected)
      {
        statistic.incrementRejected(server.getRequest(requestNumber)->getUserNumber());
      }
      statistic.addBufferTime(step.time);
      statistic.addBufferStep(step);
      statistic.addStep(step);

      for (int i = 0; i < statistic.getExecAmount(); i++)
      {
        if (distributor.getExecuterCheck().isAvaliable(server.getRequest(requestNumber)->getStartTime(), statistic, step))
        {
          distributor.getExecuter().getRequest(buffer.getRequest(step), step);
          statistic.addExecTime(step.time);
          statistic.addExecStep(step);
          statistic.addStep(step);

          break;
        }
      }

      requestNumber++;
    }
    else {
      for (int i = 0; i < statistic.getExecAmount() && !buffer.isEmpty(); i++)
      {
        if (distributor.getExecuterCheck().isAvaliable(statistic, step))
        {
          distributor.getExecuter().getRequest(buffer.getRequest(step), step);
          statistic.addExecTime(step.time);
          statistic.addExecStep(step);
          statistic.addStep(step);
          break;
        }
      }

      if (buffer.isEmpty())
      {
        distributor.getExecuter().isAvaliable(statistic, step);
      }
    }
  }
  double endTime = 0.0;
  for (int i = 0; i < statistic.getExecAmount(); i++)
  {
    if (endTime < distributor.getExecuter().getTaskStartTime())
    {
      endTime = distributor.getExecuter().getTaskStartTime();
    }
  }
  statistic.setTotalSimulationTime(endTime);
  globalStat = statistic;
}

void table(std::vector<std::vector<std::string>>& data, int width, int hight)
{
  // Variables for building a table
//-----------------------------------------
  char DOUBLE_UPPER_LEFT_CORNER(201);
  char DOUBLE_UPPER_RIGHT_CORNER(187);
  char DOUBLE_LOVER_LEFT_CORNER(200);
  char DOUBLE_LOVER_RIGHT_CORNER(188);

  char DOUBLE_HORIZONTAL_LINE(205);
  char DOUBLE_VERTICAL_LINE(186);

  char SINGLE_HORIZONTAL_LINE(196);

  char DOUBLE_TO_DOUBLE_LEFT(204);
  char DOUBLE_TO_DOUBLE_RIGHT(185);
  char DOUBLE_TO_DOUBLE_UP(203);
  char DOUBLE_TO_DOUBLE_DOWN(202);
  char DOUBLE_TO_DOUBLE_ALL(206);

  char DOUBLE_TO_SINGLE_LEFT(199);
  char DOUBLE_TO_SINGLE_RIGHT(182);
  char DOUBLE_TO_SINGLE_ALL(215);
  //-----------------------------------------

  int tableWidth = 0;
  std::vector<size_t> columnWidth(width);


  for (int i = 0; i < width; i++)
  {
    int max = 0;
    for (int j = 0; j < hight; j++)
    {
      if ((max - int(data[j][i].size())) <= 1)
      {
        max = data[j][i].size() + 2;
      }
    }
    columnWidth[i] = max;
  }

  for (int i = 0; i < width; i++)
  {
    if (i == 0)
    {
      std::cout << " " << DOUBLE_UPPER_LEFT_CORNER << std::setfill(DOUBLE_HORIZONTAL_LINE) << std::setw(columnWidth[i] + 3) << DOUBLE_TO_DOUBLE_UP;
    }
    else if (i == width - 1)
    {
      std::cout << std::setw(columnWidth[i] + 3) << DOUBLE_UPPER_RIGHT_CORNER << "\n";
    }
    else
    {
      std::cout << std::setw(columnWidth[i] + 3) << DOUBLE_TO_DOUBLE_UP;
    }

  }
  for (int i = 0; i < width; i++)
  {
    if (i == 0)
    {
      std::cout << std::setfill(' ') << " " << DOUBLE_VERTICAL_LINE;
    }
    int size = columnWidth[i] - data[0][i].size();
    int left = round(size / 2);
    int right = size - left;
    std::cout << std::setw(left + 1) << " " << data[0][i] << std::setw(right + 1) << " " << DOUBLE_VERTICAL_LINE;
    if (i == width - 1)
    {
      std::cout << "\n";
    }
  }
  for (int i = 0; i < width; i++)
  {
    if (i == 0)
    {
      std::cout << " " << DOUBLE_TO_DOUBLE_LEFT << std::setfill(DOUBLE_HORIZONTAL_LINE) << std::setw(columnWidth[i] + 3) << DOUBLE_TO_DOUBLE_ALL;
    }
    else if (i == width - 1)
    {
      std::cout << std::setw(columnWidth[i] + 3) << DOUBLE_TO_DOUBLE_RIGHT << "\n";
    }
    else
    {
      std::cout << std::setw(columnWidth[i] + 3) << DOUBLE_TO_DOUBLE_ALL;
    }

  }

  for (int i = 0; i < hight - 1; i++)
  {
    for (int j = 0; j < width; j++)
    {
      int size = columnWidth[j] - data[i + 1][j].size();
      int left = round(size / 2);
      int right = size - left;
      if (j == 0)
      {
        std::cout << " " << DOUBLE_VERTICAL_LINE << std::setfill(' ');
      }
      std::cout << std::setw(left + 1) << " " << data[i + 1][j] << std::setw(right + 1) << " " << DOUBLE_VERTICAL_LINE;
      if (j == width - 1)
      {
        std::cout << "\n";
      }
    }
    if (i == hight - 2)
    {
      for (int j = 0; j < width; j++)
      {
        if (j == 0)
        {
          std::cout << " " << DOUBLE_LOVER_LEFT_CORNER << std::setfill(DOUBLE_HORIZONTAL_LINE) << std::setw(columnWidth[j] + 3) << DOUBLE_TO_DOUBLE_DOWN;
        }
        else if (j == width - 1)
        {
          std::cout << std::setw(columnWidth[j] + 3) << DOUBLE_LOVER_RIGHT_CORNER << "\n";
        }
        else
        {
          std::cout << std::setw(columnWidth[j] + 3) << DOUBLE_TO_DOUBLE_DOWN;
        }
      }
    }
    else
    {
      for (int j = 0; j < width; j++)
      {
        if (j == 0)
        {
          std::cout << " " << DOUBLE_TO_SINGLE_LEFT << std::setfill(SINGLE_HORIZONTAL_LINE) << std::setw(columnWidth[j] + 3) << DOUBLE_TO_SINGLE_ALL;
        }
        else if (j == width - 1)
        {
          std::cout << std::setw(columnWidth[j] + 3) << DOUBLE_TO_SINGLE_RIGHT << "\n";
        }
        else
        {
          std::cout << std::setw(columnWidth[j] + 3) << DOUBLE_TO_SINGLE_ALL;
        }
      }
    }
  }
}

void fillDataStat(std::vector<std::vector<std::string>>& data)
{
  std::vector<std::string> headers = { "       ", " Generated ", " Processed ", " Rejected ", " Reject probability ", " Time in system ", " Waiting dispersion ", " Servicing dispersion " };
  data[0] = headers;
  for (int i = 1; i <= globalStat.getUserAmount(); i++)
  {
    std::string userStr = " User #" + std::to_string(i);
    userStr.append(" ");
    data[i][0] = userStr;

    int generated = globalStat.getProcessed(i - 1) + globalStat.getRejected(i - 1);
    data[i][1] = std::to_string(generated);

    int proc = globalStat.getProcessed(i - 1);
    data[i][2] = std::to_string(proc);

    int rej = globalStat.getRejected(i - 1);
    data[i][3] = std::to_string(rej);

    double rejectProb = globalStat.getRejectProbality(i - 1);
    data[i][4] = std::to_string(round(rejectProb * 10000) / 10000).substr(0, 6);

    double timeInSys = globalStat.getSimulationTime(i - 1);
    data[i][5] = std::to_string(round(timeInSys * 10000) / 10000).substr(0, 6);

    double waitTime = globalStat.getWaitingTimeDispersion(i - 1);
    data[i][6] = std::to_string(round(waitTime * 10000) / 10000).substr(0, 6);

    double servTime = globalStat.getServicingTimeDispersion(i - 1);
    data[i][7] = std::to_string(round(servTime * 10000) / 10000).substr(0, 6);
  }
}

void fillDataStatForGraph(std::vector<std::vector<float>>& data)
{
  for (int i = 1; i < globalStat.getUserAmount()+1; i++)
  {
    int generated = globalStat.getProcessed(i - 1) + globalStat.getRejected(i - 1);
    data[i-1][0] = float(generated);

    int proc = globalStat.getProcessed(i - 1);
    data[i-1][1] = float(proc);

    int rej = globalStat.getRejected(i - 1);
    data[i-1][2] = float(rej);

    double rejectProb = globalStat.getRejectProbality(i - 1);
    data[i-1][3] = float(round(rejectProb * 10000) / 10000);

    double timeInSys = globalStat.getSimulationTime(i - 1);
    data[i-1][4] = float(round(timeInSys * 10000) / 10000);

    double waitTime = globalStat.getWaitingTimeDispersion(i - 1);
    data[i-1][5] = float(round(waitTime * 10000) / 10000);

    double servTime = globalStat.getServicingTimeDispersion(i - 1);
    data[i-1][6] = float(round(servTime * 10000) / 10000);
  }
}

void fillDataExecUsage(std::vector<std::vector<std::string>>& data)
{
  std::vector<std::string> headers = { " Executors ", " Executor usage " };
  data[0] = headers;

  for (int i = 1; i <= globalStat.getExecAmount(); i++)
  {
    std::string userStr = " Device #" + std::to_string(i);
    userStr.append(" ");
    data[i][0] = userStr;

    double usage = globalStat.getExecUsage(i - 1);
    data[i][1] = std::to_string(round(usage * 10000) / 10000).substr(0, 6);
  }
}

void stepMode()
{
  step step = globalStat.getStepVector()[step_];

  std::string actionText = step.actionString;

  double time = step.time;

  int stepNum = step_ + 1;

  std::cout << "\n Step - " << stepNum << "/" << globalStat.getStepVector().size();
  std::cout << "\n Time - " << std::fixed << std::setprecision(5) << time;
  std::cout << "\n Step action: " << actionText << "\n\n";

  std::vector<std::vector<std::string>> data(2, std::vector<std::string>((globalStat.getUserAmount() + 1)));

  for (int i = 0; i <= globalStat.getBufferSize(); i++)
  {
    if (i == 0)
    {
      data[0][i] = "User Number:";
      data[1][i] = "Last generated request:";
      continue;
    }
    std::string buffer = "User #";
    buffer.append(std::to_string(i));
    std::string curObj;

    if (step.userStateVector[i - 1] == nullptr)
    {
      curObj = "-";
    }
    else {
      curObj = std::to_string(step.userStateVector[i - 1]->getUserNumber() + 1) + "." + std::to_string(step.userStateVector[i - 1]->getRequestNumber());
    }

    data[0][i] = buffer;
    data[1][i] = curObj;
  }
  table(data, globalStat.getUserAmount() + 1, 2);
  std::cout << "\n\n";

  std::vector<std::vector<std::string>> data1(3, std::vector<std::string>((globalStat.getBufferSize() + 1)));

  for (int i = 0; i <= globalStat.getBufferSize(); i++)
  {
    if (i == 0)
    {
      data1[0][i] = "Buffer Number:";
      data1[1][i] = "Status:";
      data1[2][i] = "Current request:";
      continue;
    }
    std::string buffer = "Buffer #";
    buffer.append(std::to_string(i));
    std::string status;
    std::string curObj;

    if (step.bufferStateVector[i - 1] == nullptr)
    {
      status = "Free";
      curObj = "-";
    }
    else {
      status = "Busy";
      curObj = std::to_string(step.bufferStateVector[i - 1]->getUserNumber() + 1) + "." + std::to_string(step.bufferStateVector[i - 1]->getRequestNumber());
    }

    data1[0][i] = buffer;
    data1[1][i] = status;
    data1[2][i] = curObj;
  }
  table(data1, globalStat.getBufferSize() + 1, 3);
  std::cout << "\n\n";

  std::vector<std::vector<std::string>> data2(3, std::vector<std::string>((globalStat.getExecAmount() + 1)));

  for (int i = 0; i <= globalStat.getExecAmount(); i++)
  {
    if (i == 0)
    {
      data2[0][i] = "Executor Number:";
      data2[1][i] = "Status:";
      data2[2][i] = "Current request:";
      continue;
    }
    std::string executor = "Executor #";
    executor.append(std::to_string(i));
    std::string status;
    std::string curObj;

    if (step.execStateVector[i - 1] == nullptr)
    {
      status = "Free";
      curObj = "-";
    }
    else {
      status = "Busy";
      curObj = std::to_string(step.execStateVector[i - 1]->getUserNumber() + 1) + "." + std::to_string(step.execStateVector[i - 1]->getRequestNumber());
    }

    data2[0][i] = executor;
    data2[1][i] = status;
    data2[2][i] = curObj;
  }
  table(data2, globalStat.getExecAmount() + 1, 3);

  if (step_ == globalStat.getStepVector().size() - 1)
  {
    next = false;
  }
  else
  {
    next = true;
  }

  if (step_ == 0)
  {
    back = false;
  }
  else
  {
    back = true;
  }
}

int drawGraph(int userCount, std::vector<std::vector<float>> data)
{
  RenderWindow window(VideoMode(W, H), "Generated");
  RenderWindow window2(VideoMode(W, H), "Processed");
  RenderWindow window3(VideoMode(W, H), "Rejected");
  RenderWindow window4(VideoMode(W, H), "Reject probabuluty");
  RenderWindow window5(VideoMode(W, H), "Time in system");
  RenderWindow window6(VideoMode(W, H), "Waiting dispersion");
  RenderWindow window7(VideoMode(W, H), "Servicing dispersion");

  int x0 = W / 2;
  int y0 = H / 2;
  CircleShape point(2.f);
  point.setFillColor(Color::Blue);

  int o1 = -10;
  int o2 = 10;
  float c = 100;
  int mass = ((o1) * (-1) + o2) * c + 1;
  int sc = 30;

  RectangleShape line[40];
  for (int i = 0; i < 40; i++) {
    line[i].setSize(Vector2f(1, 20));
    line[i].setFillColor(Color::Black);

    if (i < 20) {
      if (i < 10)
        line[i].setPosition(x0 - (i + 1) * sc, y0 - 10);
      else
        line[i].setPosition(x0 + (i - 9) * sc, y0 - 10);
    }
    else {
      line[i].setRotation(90);
      if (i < 30)
        line[i].setPosition(x0 + 10, y0 + (i - 30) * sc);
      else
        line[i].setPosition(x0 + 10, y0 + (i - 29) * sc);
    }
  }

  RectangleShape OsX(Vector2f(W, 1));
  OsX.setFillColor(Color::Black);
  OsX.setPosition(0, y0);

  RectangleShape OsY(Vector2f(1, H));
  OsY.setFillColor(Color::Black);
  OsY.setPosition(x0, 0);

  RectangleShape strel[4];
  for (int i = 0; i < 4; i++) {
    strel[i].setSize(Vector2f(1, 25));
    strel[i].setFillColor(Color::Black);

    if (i < 2)
      strel[i].setPosition(x0, 0);
    else
      strel[i].setPosition(W, y0);
  }
  strel[0].setRotation(25);
  strel[1].setRotation(-25);
  strel[2].setRotation(60);
  strel[3].setRotation(-250);

  while (window.isOpen())
  {
    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();
    }

    window.clear(Color::White);
    window2.clear(Color::White);
    window3.clear(Color::White);
    window4.clear(Color::White);
    window5.clear(Color::White);
    window6.clear(Color::White);
    window7.clear(Color::White);
    window.draw(OsX);
    window.draw(OsY);
    window2.draw(OsX);
    window2.draw(OsY);
    window3.draw(OsX);
    window3.draw(OsY);
    window4.draw(OsX);
    window4.draw(OsY);
    window5.draw(OsX);
    window5.draw(OsY);
    window6.draw(OsX);
    window6.draw(OsY);
    window7.draw(OsX);
    window7.draw(OsY);
    for (int i = 0; i < 4; i++)
    {
      window.draw(strel[i]);
      window2.draw(strel[i]);
      window3.draw(strel[i]);
      window4.draw(strel[i]);
      window5.draw(strel[i]);
      window6.draw(strel[i]);
      window7.draw(strel[i]);
    }
    for (int i = 0; i < 40; i++)
    {
      if (i != 19 && i != 20)
      {
        window.draw(line[i]);
        window2.draw(line[i]);
        window3.draw(line[i]);
        window4.draw(line[i]);
        window5.draw(line[i]);
        window6.draw(line[i]);
        window7.draw(line[i]);
      }
    }
    int userNum = 0, graphNum = 0;
    for (int i = 0; i < mass; i++) {
      float x = o1 + i / c;
      float y;
      while (data[userNum][graphNum] > 10.0)
      {
        data[userNum][graphNum] /= 10.0;
      }

      if (i > (userNum+1) * mass / userCount)
      {
        userNum++;
      }
      y = data[userNum][graphNum];

      float x1 = x0 + x * sc;
      float y1 = y0 - y * sc;

      point.setPosition(x1, y1);
      window.draw(point);
    }
    window.display();

    graphNum++;
    userNum = 0;
    for (int i = 0; i < mass; i++) {
      float x = o1 + i / c;
      float y;
      while (data[userNum][graphNum] > 10.0)
      {
        data[userNum][graphNum] /= 10.0;
      }

      if (i > (userNum + 1) * mass / userCount)
      {
        userNum++;
      }
      y = data[userNum][graphNum];

      float x1 = x0 + x * sc;
      float y1 = y0 - y * sc;

      point.setPosition(x1, y1);
      window2.draw(point);
    }
    window2.display();

    graphNum++;
    userNum = 0;
    for (int i = 0; i < mass; i++) {
      float x = o1 + i / c;
      float y;
      while (data[userNum][graphNum] > 10.0)
      {
        data[userNum][graphNum] /= 10.0;
      }

      if (i > (userNum + 1) * mass / userCount)
      {
        userNum++;
      }
      y = data[userNum][graphNum];

      float x1 = x0 + x * sc;
      float y1 = y0 - y * sc;

      point.setPosition(x1, y1);
      window3.draw(point);
    }
    window3.display();

    graphNum++;
    userNum = 0;
    for (int i = 0; i < mass; i++) {
      float x = o1 + i / c;
      float y;
      while (data[userNum][graphNum] > 10.0)
      {
        data[userNum][graphNum] /= 10.0;
      }

      if (i > (userNum + 1) * mass / userCount)
      {
        userNum++;
      }
      y = data[userNum][graphNum];

      float x1 = x0 + x * sc;
      float y1 = y0 - y * sc;

      point.setPosition(x1, y1);
      window4.draw(point);
    }
    window4.display();

    graphNum++;
    userNum = 0;
    for (int i = 0; i < mass; i++) {
      float x = o1 + i / c;
      float y;
      while (data[userNum][graphNum] > 10.0)
      {
        data[userNum][graphNum] /= 10.0;
      }

      if (i > (userNum + 1) * mass / userCount)
      {
        userNum++;
      }
      y = data[userNum][graphNum];

      float x1 = x0 + x * sc;
      float y1 = y0 - y * sc;

      point.setPosition(x1, y1);
      window5.draw(point);
    }
    window5.display();

    graphNum++;
    userNum = 0;
    for (int i = 0; i < mass; i++) {
      float x = o1 + i / c;
      float y;
      while (data[userNum][graphNum] > 10.0)
      {
        data[userNum][graphNum] /= 10.0;
      }

      if (i > (userNum + 1) * mass / userCount)
      {
        userNum++;
      }
      y = data[userNum][graphNum];

      float x1 = x0 + x * sc;
      float y1 = y0 - y * sc;

      point.setPosition(x1, y1);
      window6.draw(point);
    }
    window6.display();

    graphNum++;
    userNum = 0;
    for (int i = 0; i < mass; i++) {
      float x = o1 + i / c;
      float y;
      while (data[userNum][graphNum] > 10.0)
      {
        data[userNum][graphNum] /= 10.0;
      }

      if (i > (userNum + 1) * mass / userCount)
      {
        userNum++;
      }
      y = data[userNum][graphNum];

      float x1 = x0 + x * sc;
      float y1 = y0 - y * sc;

      point.setPosition(x1, y1);
      window7.draw(point);
    }
    window7.display();
  }
  return 0;
}