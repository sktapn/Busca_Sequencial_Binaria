#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void selectionSort(int arr[], int n, int *comparisons, int *swaps);
void insertionSort(int arr[], int n, int *comparisons, int *swaps);
void bubbleSort(int arr[], int n, int *comparisons, int *swaps);
void heapSort(int arr[], int n, int *comparisons, int *swaps);
void radixSort(int arr[], int n);
void quickSort(int arr[], int low, int high, int *comparisons, int *swaps);
void mergeSort(int arr[], int l, int r, int *comparisons, int *swaps);

// Funções auxiliares para ordenação
int getMax(int arr[], int n);
void countSort(int arr[], int n, int exp);
int partition(int arr[], int low, int high, int *comparisons, int *swaps);
void merge(int arr[], int l, int m, int r, int *comparisons, int *swaps);

int binarySearch(int arr[], int l, int r, int x, int *comparisons);
int sequentialSearch(int arr[], int n, int x, int *comparisons);

void generateRandomNumbers(int arr[], int n);
void saveToFile(int arr[], int n, const char *filename);
void readFromFile(int arr[], int n, const char *filename);

void printMenu();
void testSortingAlgorithm(int arr[], int n, const char *algorithmName,
                          void (*sortFunc)(int[], int, int*, int*),
                          const char *inputFile, const char *outputFile);

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int choice;
    int n, searchValue;
    int *arr = NULL;

    while (1) {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // Gerar os 4 arquivos com sequências aleatórias
                printf("\n=== GERAR ARQUIVOS ===\n");
                int sizes[] = {100, 1000, 10000, 100000};
                int numSizes = sizeof(sizes) / sizeof(sizes[0]);

                for (int i = 0; i < numSizes; i++) {
                    n = sizes[i];
                    arr = (int *)malloc(n * sizeof(int));
                    if (arr == NULL) {
                        printf("Erro: Falha ao alocar memória!\n");
                        exit(1);
                    }
                    generateRandomNumbers(arr, n);
                    char filename[50];
                    sprintf(filename, "random_%d.txt", n); // Nome ajustado para consistência com case 2
                    saveToFile(arr, n, filename);
                    printf("Arquivo '%s' com %d números gerado com sucesso!\n", filename, n);
                    free(arr);
                }
                break;
            }

            case 2: {
                // Ordenar os arquivos e medir desempenho dos algoritmos
                printf("\n=== ORDENAR ARQUIVOS ===\n");
                printf("Digite o tamanho do arquivo a ser ordenado (100, 1000, 10000 ou 100000): ");
                scanf("%d", &n);

                char inputFile[50];
                sprintf(inputFile, "random_%d.txt", n);
                printf("\nLendo arquivo: %s\n", inputFile);

                // Aloca array para os números
                arr = (int *)malloc(n * sizeof(int));
                if (arr == NULL) {
                    printf("Erro: Falha ao alocar memória!\n");
                    exit(1);
                }

                // Para cada algoritmo, lê o arquivo, ordena e salva o resultado
                char outputFile[50];

                // Selection Sort
                sprintf(outputFile, "selection_sort_%d.txt", n);
                testSortingAlgorithm(arr, n, "Selection Sort", selectionSort, inputFile, outputFile);

                // Insertion Sort
                sprintf(outputFile, "insertion_sort_%d.txt", n);
                testSortingAlgorithm(arr, n, "Insertion Sort", insertionSort, inputFile, outputFile);

                // Bubble Sort
                sprintf(outputFile, "bubble_sort_%d.txt", n);
                testSortingAlgorithm(arr, n, "Bubble Sort", bubbleSort, inputFile, outputFile);

                // Heap Sort
                sprintf(outputFile, "heap_sort_%d.txt", n);
                testSortingAlgorithm(arr, n, "Heap Sort", heapSort, inputFile, outputFile);

                // Quick Sort
                sprintf(outputFile, "quick_sort_%d.txt", n);
                testSortingAlgorithm(arr, n, "Quick Sort",
                    (void (*)(int[], int, int*, int*))quickSort, inputFile, outputFile);

                // Merge Sort
                sprintf(outputFile, "merge_sort_%d.txt", n);
                testSortingAlgorithm(arr, n, "Merge Sort",
                    (void (*)(int[], int, int*, int*))mergeSort, inputFile, outputFile);

                // Radix Sort (não contabiliza comparações e trocas)
                readFromFile(arr, n, inputFile);
                clock_t start = clock();
                radixSort(arr, n);
                clock_t end = clock();
                double time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
                sprintf(outputFile, "radix_sort_%d.txt", n);
                saveToFile(arr, n, outputFile);
                printf("ALGORITMO: Radix Sort\n");
                printf("Tamanho: %d\n", n);
                printf("Tempo: %.6f s (%.2f ms)\n", time_sec, time_sec * 1000);
                printf("Arquivo Ordenado: ok\n\n");

                free(arr);
                break;
            }

            case 3: {
                // Busca de um valor usando busca sequencial e busca binária
                printf("\n=== BUSCA DE VALOR ===\n");
                printf("Digite o tamanho do arquivo (100, 1000, 10000 ou 100000): ");
                scanf("%d", &n);

                arr = (int *)malloc(n * sizeof(int));
                if (arr == NULL) {
                    printf("Erro: Falha ao alocar memória!\n");
                    exit(1);
                }
                char filename[50];
                sprintf(filename, "random_%d.txt", n);
                readFromFile(arr, n, filename);

                printf("Digite o valor a ser buscado: ");
                scanf("%d", &searchValue);

                // Busca Sequencial (na lista não ordenada)
                int seqComparisons = 0;
                int seqResult = sequentialSearch(arr, n, searchValue, &seqComparisons);

                // Para busca binária, é preciso ordenar o array primeiro
                int comparisons = 0, swaps = 0;
                quickSort(arr, 0, n - 1, &comparisons, &swaps);

                int binComparisons = 0;
                int binResult = binarySearch(arr, 0, n - 1, searchValue, &binComparisons);

                printf("\nRESULTADO DA BUSCA:\n");
                printf("Busca Sequencial: Valor = %d, Comparações = %d, Índice = %d\n",
                       searchValue, seqComparisons, seqResult);
                printf("Busca Binária   : Valor = %d, Comparações = %d, Índice = %d\n",
                       searchValue, binComparisons, binResult);

                free(arr);
                break;
            }

            case 0:
                printf("Saindo...\n");
                return 0;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}

void printMenu() {
    printf("\n=== MENU ===\n");
    printf("1 - Gerar arquivos com sequências aleatórias\n");
    printf("2 - Ordenar arquivos e medir desempenho\n");
    printf("3 - Buscar valor (Busca Sequencial e Busca Binária)\n");
    printf("0 - Sair\n");
    printf("Opção: ");
}

void testSortingAlgorithm(int arr[], int n, const char *algorithmName,
                          void (*sortFunc)(int[], int, int*, int*),
                          const char *inputFile, const char *outputFile) {
    readFromFile(arr, n, inputFile);

    int comparisons = 0, swaps = 0;
    clock_t start = clock();

    sortFunc(arr, n, &comparisons, &swaps);

    clock_t end = clock();
    double time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("ALGORITMO: %s\n", algorithmName);
    printf("Tamanho: %d\n", n);
    printf("Comparações: %d\n", comparisons);
    printf("Trocas: %d\n", swaps);
    printf("Tempo: %.6f s (%.2f ms)\n", time_sec, time_sec * 1000);
    printf("Arquivo Ordenado: ok\n\n");

    saveToFile(arr, n, outputFile);
}

void selectionSort(int arr[], int n, int *comparisons, int *swaps) {
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            (*comparisons)++;
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        if (min_idx != i) { // Só conta troca se realmente houve movimentação
            (*swaps)++;
            int temp = arr[min_idx];
            arr[min_idx] = arr[i];
            arr[i] = temp;
        }
    }
}

void insertionSort(int arr[], int n, int *comparisons, int *swaps) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) { // Condição movida para o while
            (*comparisons)++;            // Contagem só dentro do loop
            arr[j + 1] = arr[j];
            (*swaps)++;
            j--;
        }
        arr[j + 1] = key;
    }
}

void bubbleSort(int arr[], int n, int *comparisons, int *swaps) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            (*comparisons)++;
            if (arr[j] > arr[j + 1]) {
                (*swaps)++;
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void heapify(int arr[], int n, int i, int *comparisons, int *swaps) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        (*comparisons)++;
        if (arr[left] > arr[largest])
            largest = left;
    }
    if (right < n) {
        (*comparisons)++;
        if (arr[right] > arr[largest])
            largest = right;
    }
    if (largest != i) {
        (*swaps)++;
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest, comparisons, swaps);
    }
}

void heapSort(int arr[], int n, int *comparisons, int *swaps) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, comparisons, swaps);

    for (int i = n - 1; i > 0; i--) {
        (*swaps)++;
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0, comparisons, swaps);
    }
}

int getMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

void countSort(int arr[], int n, int exp) {
    int *output = (int *)malloc(n * sizeof(int));
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];

    free(output);
}

void radixSort(int arr[], int n) {
    int max = getMax(arr, n);
    for (int exp = 1; max / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

int partition(int arr[], int low, int high, int *comparisons, int *swaps) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        (*comparisons)++;
        if (arr[j] < pivot) {
            i++;
            (*swaps)++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    (*swaps)++;
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

void quickSort(int arr[], int low, int high, int *comparisons, int *swaps) {
    if (low < high) {
        int pi = partition(arr, low, high, comparisons, swaps);
        quickSort(arr, low, pi - 1, comparisons, swaps);
        quickSort(arr, pi + 1, high, comparisons, swaps);
    }
}

void merge(int arr[], int l, int m, int r, int *comparisons, int *swaps) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        (*comparisons)++;
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r, int *comparisons, int *swaps) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, comparisons, swaps);
        mergeSort(arr, m + 1, r, comparisons, swaps);
        merge(arr, l, m, r, comparisons, swaps);
    }
}

int binarySearch(int arr[], int l, int r, int x, int *comparisons) {
    while (l <= r) {
        (*comparisons)++;
        int m = l + (r - l) / 2;
        if (arr[m] == x)
            return m;
        if (arr[m] < x)
            l = m + 1;
        else
            r = m - 1;
    }
    return -1;
}

int sequentialSearch(int arr[], int n, int x, int *comparisons) {
    for (int i = 0; i < n; i++) {
        (*comparisons)++;
        if (arr[i] == x)
            return i;
    }
    return -1;
}

void generateRandomNumbers(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100000; // Números entre 0 e 99999
}

void saveToFile(int arr[], int n, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro: Não foi possível abrir o arquivo %s!\n", filename);
        return;
    }
    for (int i = 0; i < n; i++)
        fprintf(file, "%d\n", arr[i]);
    fclose(file);
}

void readFromFile(int arr[], int n, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro: Não foi possível abrir o arquivo %s!\n", filename);
        exit(1);
    }
    for (int i = 0; i < n; i++)
        fscanf(file, "%d", &arr[i]);
    fclose(file);
}