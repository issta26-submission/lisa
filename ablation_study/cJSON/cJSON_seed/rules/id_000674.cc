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
//<ID> 674
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *child1 = cJSON_CreateString("shared_value");
    cJSON *child2 = cJSON_CreateString("shared_value");
    cJSON *nested = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddItemToObject(root, "child", child1);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(nested, "list", arr);
    cJSON_AddItemToArray(arr, num);

    // step 3: Operate / Validate
    equal = cJSON_Compare(child1, child2, 1);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_Minify(scratch);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(child2);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}