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
//<ID> 865
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
    cJSON *first_ref = NULL;
    cJSON *num_created = NULL;
    cJSON *num_added = NULL;
    cJSON *got_item0 = NULL;
    cJSON *got_item1 = NULL;
    char *printed = NULL;
    const char *ref_str = "initial_ref";
    double value_from_array = 0.0;
    double value_from_object = 0.0;
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_added = cJSON_AddNumberToObject(root, "count", 7.0);
    arr = cJSON_AddArrayToObject(root, "items");
    first_ref = cJSON_CreateStringReference(ref_str);
    cJSON_AddItemToArray(arr, first_ref);

    // step 3: Operate / Validate
    num_created = cJSON_CreateNumber(3.14);
    cJSON_InsertItemInArray(arr, 1, num_created);
    printed = cJSON_PrintUnformatted(root);
    got_item0 = cJSON_GetArrayItem(arr, 0);
    got_item1 = cJSON_GetArrayItem(arr, 1);
    value_from_array = cJSON_GetNumberValue(got_item1);
    value_from_object = cJSON_GetNumberValue(num_added);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(scratch, 0, sizeof(scratch));

    // API sequence test completed successfully
    (void)got_item0;
    (void)value_from_array;
    (void)value_from_object;
    return 66;
}