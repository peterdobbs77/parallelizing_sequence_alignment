#!/bin/bash
(./omp_align 400 2) > omp_out/omp_out_400_np2.txt
(./omp_align 400 4) > omp_out/omp_out_400_np4.txt
(./omp_align 400 8) > omp_out/omp_out_400_np8.txt
(./omp_align 400 16) > omp_out/omp_out_400_np16.txt
(./omp_align 400 32) > omp_out/omp_out_400_np32.txt
