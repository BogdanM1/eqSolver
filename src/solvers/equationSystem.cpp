#include "equationSystem.h"

void equationSystem::reserve()
{
    rows.reserve(nonzero_count); 	
    cols.reserve(nonzero_count);
    vals.reserve(nonzero_count);
    rhs.reserve(eq_count);
}

void equationSystem::clear()
{
    rows.clear();	
    cols.clear();
    vals.clear();
    rhs.clear();
}


void equationSystem::print(FILE* out)
{
	fprintf(out, "%%MatrixMarket matrix coordinate real general\n");
    fprintf(out,"%d %d %d\n",eq_count, eq_count, nonzero_count);
	for(int i=0; i < nonzero_count; i++) fprintf(out,"%d %d %lf\n", rows[i], cols[i], vals[i]);
	for(int i=0; i < eq_count; i++) fprintf(out,"%lf\n", rhs[i]);
}


equationSystem& equationSystem::operator=(equationSystem& r)
{
	clear();
	eq_count 	  = r.eq_count;
    nonzero_count = r.nonzero_count;
	for(int i=0; i< nonzero_count; i++)
	{
		rows.push_back(r.rows[i]);
		cols.push_back(r.cols[i]);
		vals.push_back(r.vals[i]);
	}
	for(int i=0; i < eq_count; i++)
		rhs.push_back(r.rhs[i]);	
	return(*this);
}

