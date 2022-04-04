__kernel void bitonic_merge_int(__global int *sequence, int subSeqSize, int step) {

	const int i = get_local_id (0);
    const int size = get_local_size (0);

    __local int seq[size];

    for (int j = 0; j < size; ++j)
        seq = sequence[j];

	if (i % (step * 2) < step) {

		int upORdown = 1;

		if ((i / subSeqSize) % 2 == 1)
			upORdown = 0;

		int first  = seq[i];
		int second = seq[i + step];

		if ((first > second) && upORdown) {

			seq[i] = second;
			seq[i + step] = first;
		}
		else if ((first < second) && !upORdown) {

			seq[i] = second;
			seq[i + step] = first;
		}
	}
}