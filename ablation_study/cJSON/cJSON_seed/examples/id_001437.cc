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
//<ID> 1437
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
    cJSON_AddNumberToObject(stats, "errors", 2.0);
    cJSON_AddNumberToObject(stats, "warnings", 5.0);
    cJSON_AddItemToObject(root, "stats", stats);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *score = cJSON_CreateNumber(99.5);
    cJSON_AddItemToObject(root, "score", score);

    // step 3: Operate & Validate
    cJSON *dup_score = cJSON_Duplicate(score, 0);
    cJSON_bool is_num = cJSON_IsNumber(dup_score);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), is_num);

    // step 4: Cleanup
    cJSON_Delete(dup_score);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}