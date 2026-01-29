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
//<ID> 997
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *n_insert = NULL;
    char *printed = NULL;
    cJSON_bool add_ok = 0;
    cJSON_bool insert_ok = 0;
    cJSON_bool is_array = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    values = cJSON_CreateArray();
    add_ok = cJSON_AddItemToObject(root, "values", values);
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 3: Operate / Validate
    n_insert = cJSON_CreateNumber(20.0);
    insert_ok = cJSON_InsertItemInArray(values, 1, n_insert);
    is_array = cJSON_IsArray(values);
    printed = cJSON_PrintBuffered(root, 64, 1);
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 2));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}