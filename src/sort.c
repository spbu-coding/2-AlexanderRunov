void sort(int *numbers, int numbers_amount) {
    for (int i = 0; i < numbers_amount; i++) {
        for (int j = i + 1; j < numbers_amount; j++) {
            if (numbers[i] > numbers[j]) {
                int temp = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = temp;
            }
        }
    }
}