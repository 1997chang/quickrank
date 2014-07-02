#ifndef __MAP_SCORER_HPP__
#define __MAP_SCORER_HPP__

#include <cstdio>

#include "metric/metricscorer.hpp"
#include "utils/strutils.hpp" // rtnode string

class mapscorer : public metricscorer {
	private:
		unsigned int *relevantdocs, nrelevantdocs;
	public:
		mapscorer(const unsigned int kval=0) : relevantdocs(NULL), nrelevantdocs(0) {
			k=kval;
		}
		const char *whoami() const {
			return "MAP";
		}
		float compute_score(const rnklst &rl) {
			float ap = 0.0f;
			unsigned int count = 0;
			//compute score for relevant labels
			for(unsigned int i=0; i<rl.size; ++i)
				if(rl.labels[i]>0.0f)
					ap += (++count)/(i+1.0f);
			count = (rl.id<nrelevantdocs && relevantdocs[rl.id]>count) ? relevantdocs[rl.id] : count;
			return count==0 ? 0.0f : ap/count;
		}
		fsymmatrix *swap_change(const rnklst &rl) {
			int labels[rl.size];
			int relcount[rl.size];
			unsigned int count = 0;
			for(unsigned int i=0; i<rl.size; ++i) {
				if(rl.labels[i]>0.0f) //relevant if true
					labels[i] = 1,
					++count;
				else
					labels[i] = 0;
				relcount[i] = count;
			}
			count = (rl.id<nrelevantdocs && relevantdocs[rl.id]>count) ? relevantdocs[rl.id] : count;
			fsymmatrix *changes = new fsymmatrix(rl.size);
			if(count==0)
				return changes; //all zeros
			#pragma omp parallel for
			for(unsigned int i=0; i<rl.size-1; ++i)
				for(unsigned int j=i+1; j<rl.size; ++j)
					if(labels[i]!=labels[j]) {
						const int diff = labels[j]-labels[i];
						float change = ((relcount[i]+diff)*labels[j]-relcount[i]*labels[i])/(i+1.0f);
						for(unsigned int k=i+1; k<j; ++k)
							if(labels[k]>0) change += (relcount[k]+diff)/(k+1.0f);
						change += (-relcount[j]*diff)/(j+1.0f);
						changes->at(i,j) = change/count;
					}
			return changes;
		}

};

#endif