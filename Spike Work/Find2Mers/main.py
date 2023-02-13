"""
Finding Microsatellites in a string using a dictionary
"""


def microsat_dict_func(genome, repeat_length):
    # Initialising dict
    microsat_dict = {}

    for i in range(0, len(genome), repeat_length):
        microsat = genome[i:i + repeat_length]
        # Checks if microsatellite is in dict
        if microsat in microsat_dict:
            microsat_dict[microsat] = microsat_dict[microsat] + 1

        # Adding microsatellite to dictionary if not already in the dictionary
        else:
            microsat_dict[microsat] = 1
    return microsat_dict


genome = "AGTCAGTCAGTCAGTC"
microsat_length = 2
print(microsat_dict_func(genome, microsat_length))
