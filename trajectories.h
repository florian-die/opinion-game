#ifndef __TRAJECTORIES__
#define __TRAJECTORIES__

const extern double PERSISTENCE_DURATION;
extern int TRAJECTORY_WIDTH;

const extern int TOP_BOT_MARGIN;

const extern int RANGE_ALPHA;
const extern int RANGE_EDGE_WIDTH;

int count_persitent_points();

void render_trajectories();

#endif