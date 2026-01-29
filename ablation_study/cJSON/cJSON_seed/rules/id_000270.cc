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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.1415);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    char json_text[] = " { \"x\" : 10, \"y\" : [ 5, 6 ] } ";
    cJSON *parsed = NULL;
    cJSON *first_item = NULL;
    double first_val = 0.0;
    int arr_size = 0;
    cJSON_bool added_numbers = 0;
    cJSON_bool added_parsed = 0;

    // step 2: Setup / Configure
    added_numbers = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_Minify(json_text);
    parsed = cJSON_Parse(json_text);
    added_parsed = cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddItemToObject(root, "first_value", cJSON_CreateNumber(first_val));

    // step 4: Cleanup
    (void)arr_size;
    (void)added_numbers;
    (void)added_parsed;
    (void)first_item;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}