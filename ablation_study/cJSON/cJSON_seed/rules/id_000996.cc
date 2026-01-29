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
//<ID> 996
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *inserted = NULL;
    cJSON *meta = NULL;
    char *printed = NULL;
    int prebuffer = 128;
    cJSON_bool printed_fmt = 1;
    cJSON_bool is_array = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    values = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.23);
    num2 = cJSON_CreateNumber(4.56);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddItemToObject(root, "values", values);
    inserted = cJSON_CreateNumber(2.50);
    cJSON_InsertItemInArray(values, 1, inserted);
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("sensor-A"));

    // step 3: Operate / Validate
    is_array = cJSON_IsArray(cJSON_GetObjectItem(root, "values"));
    printed = cJSON_PrintBuffered(root, prebuffer, printed_fmt);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}