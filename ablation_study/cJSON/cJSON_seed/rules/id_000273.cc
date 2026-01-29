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
//<ID> 273
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    char *json_buf = (char *)cJSON_malloc(128);
    memset(json_buf, 0, 128);
    const char *sample = "{ \"numbers\" : [ 10 , 20 , 30 ] }";
    strcpy(json_buf, sample);
    int arr_size = 0;
    cJSON *first_item = NULL;
    double first_val = 0.0;
    double sum = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_Minify(json_buf);
    cJSON_AddItemToObject(root, "source_minified", cJSON_CreateString(json_buf));

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_item);
    sum = first_val + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(sum));

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}