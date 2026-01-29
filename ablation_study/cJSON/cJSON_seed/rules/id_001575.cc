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
//<ID> 1575
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"value\": 42, \"nested\": {\"v\": 3.14}}";
    cJSON *root = NULL;
    cJSON *value_item = NULL;
    cJSON *nested_obj = NULL;
    cJSON *nested_v = NULL;
    cJSON *dup_v = NULL;
    char *printed = NULL;
    char *copied = NULL;
    size_t printed_len = 0;
    double value_num = 0.0;
    double nested_num = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json_text);

    // step 3: Operate / Validate
    value_item = cJSON_GetObjectItem(root, "value");
    value_num = cJSON_GetNumberValue(value_item);
    nested_obj = cJSON_GetObjectItem(root, "nested");
    nested_v = cJSON_GetObjectItem(nested_obj, "v");
    nested_num = cJSON_GetNumberValue(nested_v);
    dup_v = cJSON_Duplicate(nested_v, 1);
    equal = cJSON_Compare(nested_v, dup_v, 1);
    printed = cJSON_PrintUnformatted(root);
    printed_len = (printed != NULL) ? (strlen(printed) + 1) : 1;
    copied = (char *)cJSON_malloc(printed_len);
    memset(copied, 0, printed_len);
    if (printed) memcpy(copied, printed, printed_len);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copied);
    cJSON_Delete(dup_v);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}