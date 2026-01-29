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
//<ID> 1032
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"example\",\"items\":[\"one\",\"two\",\"three\"],\"nested\":{\"key\":\"value\"}}";
    cJSON *root = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    char *printed_root = (char *)0;
    char *printed_dup = (char *)0;
    const char *name_str = (const char *)0;
    const char *first_str = (const char *)0;
    int arr_size = 0;
    cJSON_bool cmp = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON into root
    root = cJSON_Parse(json_text);

    // step 3: Configure - obtain array and string items and extract their string values and size
    items = cJSON_GetObjectItem(root, "items");
    name_item = cJSON_GetObjectItem(root, "name");
    first_item = cJSON_GetArrayItem(items, 0);
    name_str = cJSON_GetStringValue(name_item);
    first_str = cJSON_GetStringValue(first_item);
    arr_size = cJSON_GetArraySize(items);

    // step 4: Operate - duplicate the structure and produce unformatted prints
    dup = cJSON_Duplicate(root, (cJSON_bool)1);
    printed_root = cJSON_PrintUnformatted(root);
    printed_dup = cJSON_PrintUnformatted(dup);
    cmp = cJSON_Compare(root, dup, (cJSON_bool)1);

    // step 5: Validate - compute a small validation score using results from the APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(dup != (cJSON *)0) + arr_size + (int)(first_item != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(printed_root != (char *)0) + (int)(printed_dup != (char *)0) + (int)(cmp) + (int)(name_str != (const char *)0) + (int)(first_str != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - free printed buffers and delete JSON structures
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}