# Finding-Repetition-in-Genomes
The finding repetition in genomes project will develop a web-based application
for bioinformaticians so that they are able to find microsatellites in genomes.
Genomes are stored as strings in FASTA files, using string manipulation
bioinformaticians can analyse the genomes and are then able to make conclusions
about the genomes that they've collected. Microsatellites (also known as Simple
Sequence Repeats (SSRs)) are repetitive sections of DNA that repeat throughout
the genome. Microsatellites are important for bioinformatics because they can be
used to advance our understanding of biological systems, genetic diversity, and
evolutionary processes. Furthermore, microsatellites are also useful for
identifying and tracking genetic changes in populations, including those that may
be important for human health or that are associated with diseases. Most
commonly, Microsatellites are considered to be between 2 and 7 bases long. This repo details the web application for finding microsatellites.

## Using the offline version
First download the offline\_microsatfinder folder and then you are able to run the offline finder with your FASTA file. 

```bash 
python3 offline_microsatfinder/main.py <FASTAfilepath>

