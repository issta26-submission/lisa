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
//<ID> 971
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = " { \"name\" : \"tester\" , \"values\": [1,2,3] } ";
    cJSON_Minify(json);
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "flag");
    cJSON *child = cJSON_CreateObject();
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "maybe", maybe);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate
    int values_count = cJSON_GetArraySize(cJSON_GetObjectItem(root, "values"));
    cJSON_AddNumberToObject(root, "values_count", (double)values_count);
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}