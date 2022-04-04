__kernel void bitonic_merge_int(__global int *sequence, int subSeqSize, int step) {

	const int i = get_local_id (0);
    // const int size = get_local_size (0);
    __local int seq[1024];

	int groupNum = i * step * 2;
	int numElems = step * 2;

	for (int j = 0; j < numElems; ++j) 
		seq[j] = sequence[groupNum + j];
	
	barrier (CLK_LOCAL_MEM_FENCE);
    // for (int j = 0; j < 1024; ++j)
    //     seq[j] = sequence[j];
	int upORdown = 1;
	if (((i * step * 2) / subSeqSize) % 2 == 1)
			upORdown = 0;

	for (int counter = 0; counter < step; ++counter) {

		int first  = 1; seq[counter];
		int second = 1; seq[counter + step];

		if ((first > second) && upORdown) {

			seq[counter] = 1; second;
			seq[counter + step] = 1; first;
		}
		else if ((first < second) && !upORdown) {

			seq[counter] = 1; second;
			seq[counter + step] = 1; first;
		}
	}

	barrier (CLK_LOCAL_MEM_FENCE);

	for (int j = 0; j < numElems; ++j) 
		sequence[groupNum + j] = seq[j];
}