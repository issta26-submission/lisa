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
//<ID> 998
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *name = NULL;
    char *printed = NULL;
    cJSON_bool added_values = 0;
    cJSON_bool added_name = 0;
    cJSON_bool is_array = 0;
    cJSON_bool inserted_a = 0;
    cJSON_bool inserted_b = 0;
    int prebuffer = 128;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    values = cJSON_CreateArray();
    added_values = cJSON_AddItemToObject(root, "values", values);
    name = cJSON_CreateString("sensor-01");
    added_name = cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate / Validate
    is_array = cJSON_IsArray(values);
    num_a = cJSON_CreateNumber(3.14159);
    num_b = cJSON_CreateNumber(2.71828);
    inserted_a = cJSON_InsertItemInArray(values, 0, num_a);
    inserted_b = cJSON_InsertItemInArray(values, 1, num_b);
    printed = cJSON_PrintBuffered(root, prebuffer, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}