#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);

    // step 2: Configure
    cJSON *num_pi = cJSON_AddNumberToObject(stats, "pi", 3.14159);
    cJSON *num_answer = cJSON_AddNumberToObject(stats, "answer", 42.0);
    cJSON *num_e = cJSON_CreateNumber(2.71828);
    cJSON_AddItemToObject(stats, "e", num_e);

    // step 3: Operate & Validate
    cJSON *retrieved_stats = cJSON_GetObjectItem(root, "stats");
    cJSON *retrieved_pi = cJSON_GetObjectItem(retrieved_stats, "pi");
    int prebuffer = 256;
    cJSON_bool fmt = 1;
    char *printed = cJSON_PrintBuffered(root, prebuffer, fmt);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}