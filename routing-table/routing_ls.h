struct network_forwarding_tables
{
    int num_routers;
    struct forwarding_table fwd_table_arr[];
};

struct forwarding_table
{
    int router;
    int next_hop_arr[];
    int cost_arr[];
};

