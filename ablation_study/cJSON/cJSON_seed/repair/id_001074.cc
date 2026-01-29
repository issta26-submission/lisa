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
//<ID> 1074
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_AddArrayToObject(root, "numbers");
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON *ref_item = cJSON_CreateString("referenced");
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "ref_str", ref_item);

    // step 2: Configure
    cJSON_AddStringToObject(root, "title", "cJSON Sequence Test");
    cJSON *numbers_copy = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_copy", numbers_copy);

    // step 3: Operate and Validate
    char *out = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(out);
    char *out_parsed = cJSON_Print(parsed);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (int)ref_added);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(out_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}