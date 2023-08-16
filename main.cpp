#include <iostream>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include <cstdlib>
#include <ctime>
#include "src/gtime.h"
#include <vector>
#include <cstring>
#include <chrono>
#include <sstream>
#include <thread>

using namespace std;

double getRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

class SortingCore
{
public:
    // sorting methods
    enum SortType {
        QUICK_SORT,
        BUBBLE_SORT,
        HEAP_SORT
    };
    SortType sortMethod = SortType::QUICK_SORT;

    bool canStartSort = false;
    bool showGui = true;

    bool renderStepByStep = true;
    int sortStepDelay = 100;

    int datasetCount = 250;
    double maxVal = 10000.0;

    vector<double> sortDataset = vector<double>();
    sf::RenderWindow* renderWindow;

    string sortingResults = "No results";

    explicit SortingCore(sf::RenderWindow& window)
    {
        renderWindow = &window;
        CreateArray();
    }

    ~SortingCore()
    {
        sortDataset.clear();
    }

    void CreateArray(){
        sortDataset.clear();
        for (size_t i = 0; i < datasetCount; i++)
        {
            double val = getRand(0.0, maxVal);
            sortDataset.push_back(val);
        }
    }

    // Partition the array into two parts and return the index of the pivot element
    int partition(std::vector<double>& arr, int low, int high) {
        double pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; ++j) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        RenderStepWithDelay();
        return i + 1;
    }

    // Recursive function to perform Quick Sort
    void quickSort(std::vector<double>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    void bubbleSort(std::vector<double>& arr) {
        int n = arr.size();
        bool swapped;

        for (int i = 0; i < n - 1; ++i) {
            swapped = false;

            for (int j = 0; j < n - i - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            RenderStepWithDelay();

            // If no two elements were swapped in the inner loop, the array is already sorted
            if (!swapped) {
                break;
            }
        }
    }


    void heapify(std::vector<double>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] > arr[largest]) {
            largest = left;
        }

        if (right < n && arr[right] > arr[largest]) {
            largest = right;
        }

        if (largest != i) {
            std::swap(arr[i], arr[largest]);
            RenderStepWithDelay();
            heapify(arr, n, largest);
        }
    }

    void heapSort(std::vector<double>& arr) {
        int n = arr.size();

        // Build a max-heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(arr, n, i);
        }

        // Extract elements from the heap one by one
        for (int i = n - 1; i > 0; --i) {
            std::swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }

    void RenderStepWithDelay(){
        //renderWindow->clear();
        //Update(*renderWindow);
        //renderWindow->display();
        if(!renderStepByStep) return;
        sf::sleep(sf::milliseconds(sortStepDelay));
    }

    bool isSorting()
    {
        return canStartSort;
    }

    void Sort()
    {
        while (true)
        {
            if(!canStartSort) continue;
            if(sortDataset.empty()) continue;
            showGui = false;

            auto startTime = std::chrono::high_resolution_clock::now();

            switch (sortMethod)
            {
                case QUICK_SORT:
                    quickSort(sortDataset, 0, sortDataset.size() - 1);
                    break;
                case BUBBLE_SORT:
                    bubbleSort(sortDataset);
                    break;
                case HEAP_SORT:
                    heapSort(sortDataset);
                    break;
                default:
                    quickSort(sortDataset, 0, sortDataset.size() - 1);
                    break;
            }


            auto endTime = std::chrono::high_resolution_clock::now();
            // Calculate the duration
            auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
            auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
            auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

            std::stringstream stream;
            stream << "Execution time: " << duration_s << " s, " << duration_ms << " ms, (" << duration_ns << ") ns"  << std::endl;
            sortingResults = stream.str();
            canStartSort = false;
            showGui = true;
        }
    }

    void Update(sf::RenderWindow& window)
    {
        if(sortDataset.empty()) return;
        int count = sortDataset.size();
        sf::Vector2u wSize = window.getSize();

        double col_width = wSize.x / count;
        if(count > wSize.x) col_width = 1.0;

        for (size_t i = 0; i < count; i++)
        {
            double col_height = wSize.y * sortDataset[i] / maxVal;
            sf::RectangleShape rect(sf::Vector2f(col_width, col_height));
            rect.setPosition(sf::Vector2f(col_width * i, 0));

            window.draw(rect);
        }

    }
};

void DrawGui(SortingCore& sortingCore)
{
    ImGui::SetNextWindowCollapsed(!sortingCore.showGui);
    ImGui::Begin("Settings", &sortingCore.showGui);

    ImGui::Text("sortingCore.showGui: %s", sortingCore.showGui? "true": "false");
    ImGui::Text("Is sorting dataset: %s", sortingCore.isSorting()? "true": "false");
    ImGui::Text("%s", sortingCore.sortingResults.c_str());

    ImGui::Text("%s", "Dataset count");
    ImGui::InputInt("Dataset count", &sortingCore.datasetCount);

    if (ImGui::Button("Create new array")) {
        if(!sortingCore.isSorting()) sortingCore.CreateArray();
    }

    ImGui::Text("%s", "Show sorting steps:");
    ImGui::Checkbox("Show sorting steps", &sortingCore.renderStepByStep);

    if(sortingCore.renderStepByStep){
        ImGui::Text("%s", "Sort step delay (ms)");
        ImGui::InputInt("Sort step delay", &sortingCore.sortStepDelay);
    }

    if(!sortingCore.canStartSort){
        if (ImGui::Button("Quick sort")) {
            sortingCore.sortMethod = sortingCore.QUICK_SORT;
            sortingCore.canStartSort = true;
        }
        if (ImGui::Button("Bubble sort")) {
            sortingCore.sortMethod = sortingCore.BUBBLE_SORT;
            sortingCore.canStartSort = true;
        }
        if (ImGui::Button("Heap sort")) {
            sortingCore.sortMethod = sortingCore.HEAP_SORT;
            sortingCore.canStartSort = true;
        }
    }

    ImGui::End();

}

void SortingCoreInitialize(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sorting algorithms");
    window.setFramerateLimit(60);
    SortingCore sortingCore(window);

    std::thread sortingThread([&sortingCore]() {
        sortingCore.Sort();
    });
    sortingThread.detach();

    // Initialize ImGui
    ImGui::SFML::Init(window);

    // Main loop
    while (window.isOpen())
    {
        ProgramTime::RestartClock();
        sf::Event event{};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else
            {
                if (event.type == sf::Event::Resized) {
                    // Handle window resize
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::A) {
                        //sortingCore.Sort();
                    }
                }
            }
        }

        ImGui::SFML::Update(window, ProgramTime::elapsedTime);
        DrawGui(sortingCore);

        // Clear the windows
        window.clear();

        sortingCore.Update(window);
        ImGui::SFML::Render(window);

        window.display();
    }

    // Shutdown ImGui
    ImGui::SFML::Shutdown();
}

int main()
{
    srand(time(nullptr));
    SortingCoreInitialize();
    return 0;
}