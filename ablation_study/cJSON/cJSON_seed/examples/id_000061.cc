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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *numA = cJSON_CreateNumber(10.0);
    cJSON *numB = cJSON_CreateNumber(32.5);
    cJSON_AddItemToObject(root, "valueA", numA);
    cJSON_AddItemToObjectCS(config, "valueB", numB);

    // step 3: Operate & Validate
    cJSON_bool hasA = cJSON_HasObjectItem(root, "valueA");
    cJSON_bool hasB = cJSON_HasObjectItem(config, "valueB");
    cJSON *gotA = cJSON_GetObjectItem(root, "valueA");
    cJSON *gotB = cJSON_GetObjectItem(config, "valueB");
    double a_val = cJSON_GetNumberValue(gotA);
    double b_val = cJSON_GetNumberValue(gotB);
    cJSON *summary = cJSON_CreateNumber(a_val + b_val + (double)hasA + (double)hasB);
    cJSON_AddItemToObject(root, "summary", summary);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}