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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *raw_fragment = cJSON_CreateRaw("{\"raw_key\":42}");
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    char *printed = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(child, "n1", n1);
    cJSON_AddItemToObject(child, "raw", raw_fragment);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "n2", n2);

    // step 3: Operate / Validate
    equal = cJSON_Compare(child, child, 1);
    v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "n1"));
    v2 = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "n2"));
    sum = v1 + v2 + (double)equal;
    cJSON_AddNumberToObject(root, "sum", sum);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}