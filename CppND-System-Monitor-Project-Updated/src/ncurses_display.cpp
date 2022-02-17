#include "ncurses_display.h"

#include <curses.h>

#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
string NCursesDisplay::ProgressBar(float percent) {
  string result{"0%"};
  int size{50};
  float bars{percent * size};

  for (int i{0}; i < size; ++i) {
    result += i <= bars ? '|' : ' ';
  }

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  string cpuName;
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  mvwprintw(
      window, ++row, 2,
      ("Number of Processors: " + to_string(system.NumberOfCpus())).c_str());
  mvwprintw(window, ++row, 2, "CPU: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.Cpu().Utilization("cpu")).c_str());
  wattroff(window, COLOR_PAIR(1));
  for (int i = 0; i < system.NumberOfCpus(); i++) {
    cpuName = "CPU" + to_string(i) + ": ";
    mvwprintw(window, ++row, 2, cpuName.c_str());
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row, 10, "");
    cpuName = "cpu" + to_string(i);
    wprintw(window, ProgressBar(system.Cpu().Utilization(cpuName)).c_str());
    wattroff(window, COLOR_PAIR(1));
  }
  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2,
            ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(
      window, ++row, 2,
      ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
  mvwprintw(window, ++row, 2,
            ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(vector<Process>& processes,
                                      WINDOW* window, int n) {
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_id_column{18};
  int const cpu_column{28};
  int const ram_column{36};
  int const time_column{45};
  int const command_column{56};
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_id_column, "CPU[ID]");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));
  int const num_processes = int(processes.size()) > n ? n : processes.size();
  /* This line added to clear the terminal line before re write.
   * I had case that for example if the old PID 12345 and the new PID is 67 the
   * output line result 67345 and same for other columns. So I added this line
   * here to make sure that the old line is cleared before write the new line
   */
  string clearLine(window->_maxx, ' ');
  for (int i = 0; i < num_processes; ++i) {
    /* This line added to clear the terminal line before re write.
     * I had case that for example if the old PID 12345 and the new PID is 67
     * the output line result 67345 and same for other columns. So I added this
     * line here to make sure that the old line is cleared before write the new
     * line
     */
    mvwprintw(window, ++row, 1, clearLine.c_str());
    mvwprintw(window, row, pid_column, to_string(processes[i].Pid()).c_str());
    mvwprintw(window, row, user_column, processes[i].User().c_str());
    mvwprintw(window, row, cpu_id_column, processes[i].Cpu().c_str());
    float cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 4).c_str());
    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, window->_maxx - 46).c_str());
  }
}

void NCursesDisplay::Display(System& system, int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color

  int x_max{getmaxx(stdscr)};
  WINDOW* system_window = newwin(system.NumberOfCpus() + 10, x_max - 1, 0, 0);
  WINDOW* process_window =
      newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

  while (1) {
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), process_window, n);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  endwin();
}
