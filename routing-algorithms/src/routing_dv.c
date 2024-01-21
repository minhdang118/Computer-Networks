#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Every router sends information to its adjacent routers */
int update(int num_routers, int network[num_routers][4*num_routers] )
{
    for (int src_router = 0; src_router < num_routers; src_router++)
    {
        for (int dst_router = 0; dst_router < num_routers; dst_router++)
        {
            network[src_router][2*num_routers+dst_router] = network[src_router][dst_router];
            network[src_router][3*num_routers+dst_router] = network[src_router][num_routers+dst_router];
        }
    }
    return 0;
}

/* Distance Vector algorithm */
int distance_vector(int num_routers, int num_links, int* link_arr[], const char* output_filename)
{
    /* Open and write to output file */
    FILE* outp_ptr;

    if ((outp_ptr = fopen(output_filename, "w")) == NULL)
    {
        fprintf(stderr, "Unable to open output file.\n");
        exit(EXIT_FAILURE);
    }

    int network[num_routers][4*num_routers];
    int timestep = 0;

    /* Timestep 0 */
    for (int src_router = 0; src_router < num_routers; src_router++)
    {
        memset(network[src_router], -1, sizeof(network[src_router]));
        network[src_router][src_router] = src_router;
        network[src_router][num_routers + src_router] = 0;

        for (int i = 0; i < num_links; i++)
        {
            int adj_router = -1;
            if (link_arr[i][0] == src_router)
            {
                adj_router = link_arr[i][1];
            }
            else if (link_arr[i][1] == src_router)
            {
                adj_router = link_arr[i][0];
            }

            if (adj_router != -1)
            {
                int src_adj_cost = link_arr[i][2];
                network[src_router][adj_router] = adj_router;
                network[src_router][num_routers+adj_router] = src_adj_cost;
            }
        }
    }

    update(num_routers, network);

    /* Print timestep 0 */
    fprintf(outp_ptr, "TIMESTEP 0:\n");
    for (int src_router = 0; src_router < num_routers; src_router++)
    {
        fprintf(outp_ptr, "Router %d\n", src_router);
        for (int dst_router = 0; dst_router < num_routers; dst_router++)
        {
            fprintf(outp_ptr, "sending to %d, next hop is %d with cost %d.\n", dst_router, network[src_router][dst_router], network[src_router][num_routers+dst_router]);
        }
        fprintf(outp_ptr, "\n");
    }
    
    int changed = 1;
    while (changed == 1)
    { 
        changed = 0;
        timestep++;
        for (int src_router = 0; src_router < num_routers; src_router++)
        {
            for (int dst_router = 0; dst_router < num_routers; dst_router++)
            {
                if (src_router != dst_router)
                {
                    int next_hop = network[src_router][2*num_routers+dst_router];
                    int min_cost = network[src_router][3*num_routers+dst_router];
                    
                    for (int i = 0; i < num_links; i++)
                    {
                        int adj_router = -1;
                        if (link_arr[i][0] == src_router)
                        {
                            adj_router = link_arr[i][1];
                        }
                        else if (link_arr[i][1] == src_router)
                        {
                            adj_router = link_arr[i][0];
                        }

                        if (adj_router != -1)
                        {
                            int adj_dst_cost = network[adj_router][3*num_routers+dst_router];
                            if (adj_dst_cost != -1)
                            {
                                int src_adj_cost = link_arr[i][2];
                                int temp = src_adj_cost + adj_dst_cost;
                                if ((min_cost == -1) || (min_cost > temp))
                                {
                                    next_hop = adj_router;
                                    min_cost = temp;
                                    changed = 1;
                                }
                            }
                        }
                    }
                    network[src_router][dst_router] = next_hop;
                    network[src_router][num_routers+dst_router] = min_cost;
                } 
            }
        }

        update(num_routers, network);
        
        /* Print timestep */
        fprintf(outp_ptr, "TIMESTEP %d:\n", timestep);
        for (int src_router = 0; src_router < num_routers; src_router++)
        {
            fprintf(outp_ptr, "Router %d\n", src_router);
            for (int dst_router = 0; dst_router < num_routers; dst_router++)
            {
                fprintf(outp_ptr, "sending to %d, next hop is %d with cost %d.\n", dst_router, network[src_router][2*num_routers+dst_router], network[src_router][3*num_routers+dst_router]);
            }
            fprintf(outp_ptr, "\n");
        }
        
    }

    fclose(outp_ptr);
}

int main(int argc, char const *argv[])
{
    /* Usage */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./[exec_filename] [input_filename] [output_filename]\n");
        exit(EXIT_FAILURE);
    }

    /* Open and read input file */
    FILE* inp_ptr;
    const char* input_filename = argv[1];
    const char* output_filename = argv[2];
    int num_routers = 0;
    int num_links = 0;

    if ((inp_ptr = fopen(input_filename, "r")) == NULL)
    {
        fprintf(stderr, "Unable to open input file.\n");
        exit(EXIT_FAILURE);
    }
   
    if ((fscanf(inp_ptr, "%d", &num_routers) != 1) 
        || (fscanf(inp_ptr, "%d", &num_links) != 1))
    {   
        fprintf(stderr, "Scanning input file failed.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        int* link_arr[num_links];
        for (int i = 0; i < num_links; i++)
        {
            link_arr[i] = (int*) malloc(3 * sizeof(int));
        }

        int index = 0;
        while (index < num_links)
        {
            fscanf(inp_ptr, "%d", &link_arr[index][0]);
            fscanf(inp_ptr, "%d", &link_arr[index][1]);
            fscanf(inp_ptr, "%d", &link_arr[index][2]);
            index++;
        }
        
        fclose(inp_ptr);

        /* Run Distance Vector algorithm and print to output file*/
        distance_vector(num_routers, num_links, link_arr, output_filename);

    }
    
    return 0;
}
