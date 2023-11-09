#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int linkstate(int num_routers, int num_links, int* link_arr[], const char* output_filename)
{
    /* Open and write to output file */
    FILE* outp_ptr;

    if ((outp_ptr = fopen(output_filename, "w")) == NULL)
    {
        fprintf(stderr, "Unable to open output file.\n");
        exit(EXIT_FAILURE);
    }

    /* Loop through each router as starting router*/
    for (int src_router = 0; src_router < num_routers; src_router++)
    {
        /* 
        Array of visited routers.
        1 at index of visited, 
        0 at index of not visited. 
        */
        int visited_arr[num_routers];

        /* 
        Array of next hop routers.
        -1 at index of src_router, 
        [next_hop_router] at index of dst routers.
         */
        int next_hop_arr[num_routers];

        /* 
        Array of routing costs.
        -1 at index of routers with infinity cost, 
        0 at index of src router, 
        [cost] at index of dst routers.
         */
        int cost_arr[num_routers];

        /* Initialize Dijkstra's algorithm*/
        memset(visited_arr, 0, sizeof(visited_arr));
        memset(next_hop_arr, -1, sizeof(next_hop_arr));
        memset(cost_arr, -1, sizeof(cost_arr));

        visited_arr[src_router] = 1;
        next_hop_arr[src_router] = src_router;
        cost_arr[src_router] = 0;

        /* Update next hop and cost for adjacent routers (with src_router)*/
        for (int i = 0; i < num_links; i++)
        {
            /* Find an adjacent router through a link */
            int dst_router = -1;
            if (link_arr[i][0] == src_router)
            {
                dst_router = link_arr[i][1];
            }
            else if (link_arr[i][1] == src_router)
            {
                dst_router = link_arr[i][0];
            }

            /* Update if adjacent router exists */
            if (dst_router != -1)
            {
                next_hop_arr[dst_router] = dst_router;
                cost_arr[dst_router] = link_arr[i][2];
            }
        }

        /* Loop num_routers times */
        int count = 0;
        while (count < num_routers)
        {
            count++;

            /* Find a not visited router with minimum cost */
            int min_cost = -1;
            int min_cost_router = -1;
            for (int i = 0; i < num_routers; i++)
            {   
                /* Not src_router AND not infinity cost AND not visited */
                if ((cost_arr[i] != 0) 
                    && (cost_arr[i] != -1) 
                    && (visited_arr[i] != 1))
                {
                    /* min_cost not initialized OR current router's cost less than min_cost */
                    if ((min_cost == -1) || (cost_arr[i] < min_cost))
                    {
                        /* Update min_cost and min_cost_router */
                        min_cost = cost_arr[i];
                        min_cost_router = i;
                    }
                }
            }

            /* Visit the min_cost router */
            visited_arr[min_cost_router] = 1;

            /* Update next hop and cost for adjacent routers (with min_cost_router) */
            for (int i = 0; i < num_links; i++)
            {
                /* Find an adjacent router through a link */
                int adj_router = -1;
                if (link_arr[i][0] == min_cost_router)
                {
                    adj_router = link_arr[i][1];
                }
                else if (link_arr[i][1] == min_cost_router)
                {
                    adj_router = link_arr[i][0];
                }

                /* Update if adjacent router exists */
                if (adj_router != -1)
                {
                    /* Check that adj_router is not visited */
                    if ((visited_arr[adj_router] != 1))
                    {
                        /*temp =  min_cost + cost from min_cost_router to adj_router */
                        int temp = min_cost + link_arr[i][2];

                        /* Check that cost from src_router to adj_router is infinity OR larger than temp */
                        if ((cost_arr[adj_router] == -1) 
                            || (cost_arr[adj_router] > temp))
                        {
                            next_hop_arr[adj_router] = next_hop_arr[min_cost_router];
                            cost_arr[adj_router] = temp;
                        }
                    }
                }
            }  
        }  

        /* Write src_router's forwarding table to output file */
        fprintf(outp_ptr, "Router %d\n", src_router);
        for (int j = 0; j < num_routers; j++)
        {
            fprintf(outp_ptr, "sending to %d, next hop is %d with cost %d.\n", j, next_hop_arr[j], cost_arr[j]);
        }
        fprintf(outp_ptr, "\n");
    }

    fclose(outp_ptr);

    return 0;
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

        /* Run Link-State algorithm */
        linkstate(num_routers, num_links, link_arr, output_filename);

    }
    
    return 0;
}
