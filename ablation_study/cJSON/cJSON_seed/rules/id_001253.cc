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
//<ID> 1253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const double numbers[3] = {10.5, 20.25, 30.75};
    cJSON *root = NULL;
    cJSON *darray = NULL;
    cJSON *item = NULL;
    cJSON *raw_item = NULL;
    char *printed = NULL;
    double extracted = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    darray = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", darray);
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddNullToObject(root, "missing");
    raw_item = cJSON_AddRawToObject(root, "meta", "{\"generated\":true}");

    // step 3: Operate / Validate
    item = cJSON_GetArrayItem(darray, 1);
    extracted = cJSON_GetNumberValue(item);
    printed = cJSON_PrintUnformatted(root);
    (void)raw_item;
    (void)extracted;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}