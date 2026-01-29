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
//<ID> 1196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *str_a = (cJSON *)0;
    cJSON *str_b = (cJSON *)0;
    char *serialized = (char *)0;
    cJSON_bool compare_case_sensitive = 0;
    cJSON_bool compare_case_insensitive = 0;
    int validation_score = 0;

    // step 2: Setup - create an array container
    arr = cJSON_CreateArray();

    // step 3: Configure - create two string items and add them to the array
    str_a = cJSON_CreateString("ExampleString");
    str_b = cJSON_CreateString("examplestring");
    cJSON_AddItemToArray(arr, str_a);
    cJSON_AddItemToArray(arr, str_b);

    // step 4: Operate / Validate - compare the two string items case-sensitively and insensitively, serialize for inspection
    compare_case_sensitive = cJSON_Compare((const cJSON *const)str_a, (const cJSON *const)str_b, 1);
    compare_case_insensitive = cJSON_Compare((const cJSON *const)str_a, (const cJSON *const)str_b, 0);
    validation_score = (int)compare_case_sensitive + (int)compare_case_insensitive;
    serialized = cJSON_PrintUnformatted(arr);
    cJSON_free((void *)serialized);

    // step 5: Cleanup - delete the array (which frees attached items)
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}