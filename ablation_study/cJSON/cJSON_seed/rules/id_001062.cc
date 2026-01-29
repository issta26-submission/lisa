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
//<ID> 1062
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *widget = NULL;
    cJSON *ref_item = NULL;
    cJSON *second = NULL;
    cJSON_bool cmp = 0;
    const char *cmp_labels[2] = { "notequal", "equal" };

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    widget = cJSON_CreateObject();
    cJSON_AddStringToObject(widget, "name", "widget");
    cJSON_AddItemToObject(root, "widget", widget);
    ref_item = cJSON_CreateString("shared");
    cJSON_AddItemReferenceToArray(arr, ref_item);
    second = cJSON_CreateString("shared");

    // step 3: Operate / Validate
    cmp = cJSON_Compare((const cJSON *const)ref_item, (const cJSON *const)second, 1);
    cJSON_AddStringToObject(root, "compare_result", cmp_labels[cmp]);

    // step 4: Cleanup
    cJSON_Delete(second);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}