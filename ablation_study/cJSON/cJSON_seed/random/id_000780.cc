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
//<ID> 780
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *stats = (cJSON *)0;
    cJSON *count_num = (cJSON *)0;
    cJSON *total_num = (cJSON *)0;
    cJSON *retrieved_stats = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    double count_value = 0.0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    stats = cJSON_CreateObject();
    count_num = cJSON_CreateNumber(42.0);
    total_num = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON_AddItemToObject(stats, "count", count_num);
    cJSON_AddItemToObject(stats, "total", total_num);

    // step 4: Operate
    retrieved_stats = cJSON_GetObjectItem(root, "stats");
    retrieved_count = cJSON_GetObjectItemCaseSensitive(retrieved_stats, "count");
    count_value = cJSON_GetNumberValue(retrieved_count);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_stats != (cJSON *)0) + (retrieved_count != (cJSON *)0) + (printed != (char *)0) + (int)(count_value == 42.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}