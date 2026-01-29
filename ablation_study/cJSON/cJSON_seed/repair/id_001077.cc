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
//<ID> 1077
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddStringToObject(root, "title", "ArrayExample");

    // step 2: Configure
    const char *src = "{\"inner\":{\"name\":\"refname\",\"val\":7}}";
    cJSON *parsed = cJSON_Parse(src);
    cJSON *inner = cJSON_GetObjectItem(parsed, "inner");
    cJSON_AddItemReferenceToObject(root, "inner_ref", inner);

    // step 3: Operate
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}