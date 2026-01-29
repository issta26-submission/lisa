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
//<ID> 1647
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *numbers = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num3 = NULL;
    cJSON *numbers_ref = NULL; /* kept declared to not change signature, may remain unused */
    cJSON *found_numbers = NULL;
    char *out = NULL;
    cJSON_bool ok1 = 0;
    cJSON_bool ok2 = 0;
    cJSON_bool ok3 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    num1 = cJSON_CreateNumber(42.0);
    ok1 = cJSON_AddItemToArray(numbers, num1);
    num2 = cJSON_CreateNumber(3.14);
    ok2 = cJSON_AddItemToArray(numbers, num2);
    /* add a reference to the existing numbers array (use AddItemReferenceToObject to avoid double-free) */
    cJSON_AddItemReferenceToObject(root, "numbers_ref", numbers);

    // step 3: Operate / Validate
    found_numbers = cJSON_GetObjectItem(root, "numbers");
    num3 = cJSON_CreateNumber(7.0);
    ok3 = cJSON_AddItemToArray(found_numbers, num3);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(out);
    // API sequence test completed successfully
    return 66;
}