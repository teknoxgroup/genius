#!/usr/bin/perl
open(TESTS,"geniustests.txt") || die "can't open the geniustests.txt file";

$errors = 0;
$tests = 0;
$options = "";

while(<TESTS>) {
	if(/^OPTIONS[ 	]+(.*)$/) {
		$options = $1;
	} elsif(/^([^	]+)	+([^	]+)$/) {
		$tests++;
		open(GENIUS,"echo '$1' | ./genius $options |") ||
			die "can't open pipe!";
		print "$1\n";
		$shd=$2;

		if($rep=<GENIUS>) {
			chomp $shd;
			chomp $rep;

			print " (should be)=$shd\n";
			print " (reported)=$rep\n";
			if($rep ne $shd) {
				print "ERROR!\n";
				$errors++;
			}
		} else {
			print "ERROR! NO OUTPUT\n";
			$errors++;
		}
		print "\n";
		close(GENIUS);
	}
}

print "tests: $tests, errors: $errors\n"
