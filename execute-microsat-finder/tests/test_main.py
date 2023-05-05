from main import main


def test_read_fasta_file_one_scaffold():
    main.read_fasta_file(">scaffoldTest1\nATGATGATGATGATGATG")
    assert main.get_scaffold_dict() == {'scaffoldTest1': 'ATGATGATGATGATGATG'}


def test_read_fasta_file_scaffold_with_leading_spaces():
    main.read_fasta_file(">scaffoldTest1\n ATGATGATGATGATGATG   ")
    assert main.get_scaffold_dict() == {'scaffoldTest1': 'ATGATGATGATGATGATG'}


def test_read_fasta_file_scaffold_with_aditional_info_in_header():
    main.read_fasta_file(">scaffoldTest1 additional info added\n ATGATGATGATGATGATG   ")
    assert main.get_scaffold_dict() == {'scaffoldTest1': 'ATGATGATGATGATGATG'}

