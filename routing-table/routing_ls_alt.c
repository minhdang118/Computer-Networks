#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "routing_ls.h"

struct network_forwarding_tables* linkstate3(int num_routers, int num_links, int* link_arr[])
{
    struct network_forwarding_tables* nft;
    nft = malloc(sizeof(*nft) + )
    nft->num_routers = num_routers;
    // nft.fwd_table_arr = (struct forwarding_table *) calloc(num_routers, sizeof(struct forwarding_table));

    struct forwarding_table fta[num_routers];
    // nft.fwd_table_arr = fta;

    for (int src_router = 0; src_router < num_routers; src_router++)
    {
        struct forwarding_table* ft;
        ft->router = src_router;
        ft->next_hop_arr = (int*) calloc(num_routers, sizeof(int));
        ft->cost_arr = (int*) calloc(num_routers, sizeof(int));

        memset(ft->next_hop_arr, -1, sizeof(ft->next_hop_arr));
        memset(ft->cost_arr, -1, sizeof(ft->cost_arr));

        nft->fwd_table_arr[src_router] = ft;
    }
    

    for (int src_router = 0; src_router < num_routers; src_router++)
    {
        // struct forwarding_table ft;
        // ft.router = src_router;
        // ft.next_hop_arr = (int*) calloc(num_routers, sizeof(int));
        // ft.cost_arr = (int*) calloc(num_routers, sizeof(int));

        int visited_arr[num_routers];
        int* nha = nft.fwd_table_arr[src_router].next_hop_arr;
        int* ca = nft.fwd_table_arr[src_router].cost_arr;

        memset(visited_arr, 0, sizeof(visited_arr));
        // memset(ft.next_hop_arr, -1, sizeof(ft.next_hop_arr));
        // memset(ft.cost_arr, -1, sizeof(ft.cost_arr));

        visited_arr[src_router] = 1;
        nha[src_router] = src_router;
        ca[src_router] = 0;

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
                nha[dst_router] = dst_router;
                ca[dst_router] = link_arr[i][2];
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
                if ((ca[i] != 0) 
                    && (ca[i] != -1) 
                    && (visited_arr[i] != 1))
                {
                    /* min_cost not initialized OR current router's cost less than min_cost */
                    if ((min_cost == -1) || (ca[i] < min_cost))
                    {
                        /* Update min_cost and min_cost_router */
                        min_cost = ca[i];
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
                        if ((ca[adj_router] == -1) 
                            || (ca[adj_router] > temp))
                        {
                            nha[adj_router] = nha[min_cost_router];
                            ca[adj_router] = temp;
                        }
                    }
                }
            }  
        }  
        // fta[src_router] = ft;
        // nft.fwd_table_arr[src_router] = ft;
    }

    // nft.fwd_table_arr = fta;

    return nft;
}



struct network_forwarding_tables linkstate2(int num_routers, int num_links, int* link_arr[])
{
    struct network_forwarding_tables nft;
    nft.num_routers = num_routers;
    nft.fwd_table_arr = (struct forwarding_table *) calloc(num_routers, sizeof(struct forwarding_table));

    for (int src_router = 0; src_router < num_routers; src_router++)
    {
        struct forwarding_table ft;
        ft.router = src_router;
        ft.next_hop_arr = (int*) calloc(num_routers, sizeof(int));
        ft.cost_arr = (int*) calloc(num_routers, sizeof(int));

        int visited_arr[num_routers];

        memset(visited_arr, 0, sizeof(visited_arr));
        memset(ft.next_hop_arr, -1, sizeof(ft.next_hop_arr));
        memset(ft.cost_arr, -1, sizeof(ft.cost_arr));

        visited_arr[src_router] = 1;
        ft.next_hop_arr[src_router] = src_router;
        ft.cost_arr[src_router] = 0;

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
                ft.next_hop_arr[dst_router] = dst_router;
                ft.cost_arr[dst_router] = link_arr[i][2];
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
                if ((ft.cost_arr[i] != 0) 
                    && (ft.cost_arr[i] != -1) 
                    && (visited_arr[i] != 1))
                {
                    /* min_cost not initialized OR current router's cost less than min_cost */
                    if ((min_cost == -1) || (ft.cost_arr[i] < min_cost))
                    {
                        /* Update min_cost and min_cost_router */
                        min_cost = ft.cost_arr[i];
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
                        if ((ft.cost_arr[adj_router] == -1) 
                            || (ft.cost_arr[adj_router] > temp))
                        {
                            ft.next_hop_arr[adj_router] = ft.next_hop_arr[min_cost_router];
                            ft.cost_arr[adj_router] = temp;
                        }
                    }
                }
            }  
        }  
        // fta[src_router] = ft;
        nft.fwd_table_arr[src_router] = ft;
    }

    // nft.fwd_table_arr = fta;

    return nft;
}

int main(int argc, char const *argv[])
{
    /* Usage */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./[exec_filename] [input_filename] [output_filename]\n");
        exit(EXIT_FAILURE);
    }

    /* Read input file */
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
        // linkstate(num_routers, num_links, link_arr, output_filename);

        struct network_forwarding_tables nft = linkstate2(num_routers, num_links, link_arr);

        FILE* outp_ptr;

        if ((outp_ptr = fopen(output_filename, "w")) == NULL)
        {
            fprintf(stderr, "Unable to open output file.\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_routers; i++)
        {
            fprintf(outp_ptr, "Router %d\n", nft.fwd_table_arr[i].router);
            for (int j = 0; j < num_routers; j++)
            {
                fprintf(outp_ptr, "sending to %d, next hop is %d with cost %d.\n", j, nft.fwd_table_arr[i].next_hop_arr[j], nft.fwd_table_arr[i].cost_arr[j]);
            }
            fprintf(outp_ptr, "\n");
        }
        
        fclose(outp_ptr);

    }
    
    return 0;
}
