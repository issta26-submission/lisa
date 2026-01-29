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
//<ID> 957
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "outer_array";
    cJSON *root = (cJSON *)0;
    cJSON *outerArr = (cJSON *)0;
    cJSON *nullItem = (cJSON *)0;
    cJSON *fetchedArr = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create object, array, and a null item
    root = cJSON_CreateObject();
    outerArr = cJSON_CreateArray();
    nullItem = cJSON_CreateNull();

    // step 3: Configure - add the null item to the array and attach array to root
    add_ok1 = cJSON_AddItemToArray(outerArr, nullItem);
    add_ok2 = cJSON_AddItemToObject(root, arr_key, outerArr);

    // step 4: Operate - retrieve the array via object lookup and serialize root
    fetchedArr = cJSON_GetObjectItem(root, arr_key);
    json_str = cJSON_PrintUnformatted(root);

    // step 5: Validate - compute a simple score based on outcomes
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(fetchedArr != (cJSON *)0) + (int)(json_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete JSON structure
    if (json_str) { cJSON_free((void *)json_str); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}