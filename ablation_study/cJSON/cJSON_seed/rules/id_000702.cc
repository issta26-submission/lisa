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
//<ID> 702
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *obj2 = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = NULL;
    cJSON_bool is_true = 0;
    cJSON_bool compare_res = 0;
    double num_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "object_one", obj1);
    cJSON_AddItemToObject(root, "object_two", obj2);
    cJSON_AddItemToObject(obj1, "truth", true_item);
    num_item = cJSON_AddNumberToObject(root, "original_number", 123.456);
    cJSON_AddFalseToObject(obj2, "truth");

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(true_item);
    cJSON_AddNumberToObject(obj1, "is_true_flag", (double)is_true);
    num_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(obj2, "copied_number", num_val);
    compare_res = cJSON_Compare(obj1, obj2, 1);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(scratch, printed ? printed : "", printed_len);
    scratch[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}