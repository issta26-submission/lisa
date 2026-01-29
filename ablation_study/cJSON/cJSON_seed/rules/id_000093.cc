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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj_a = cJSON_CreateObject();
    cJSON *obj_b = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":true}");
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(5.25);
    cJSON *n3 = cJSON_CreateNumber(10.5);
    char *printed = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(obj_a, "value1", n1);
    cJSON_AddItemToObject(obj_a, "value2", n2);
    cJSON_AddItemToObject(root, "objectA", obj_a);
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON_AddItemToObject(obj_b, "value1", n3);
    cJSON_AddNumberToObject(obj_b, "value2", 5.25);

    // step 3: Operate / Validate
    equal = cJSON_Compare(obj_a, obj_b, 1);
    v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(obj_a, "value1"));
    v2 = cJSON_GetNumberValue(cJSON_GetObjectItem(obj_a, "value2"));
    sum = v1 + v2 + (double)equal;
    cJSON_AddNumberToObject(root, "sum", sum);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(obj_b);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}