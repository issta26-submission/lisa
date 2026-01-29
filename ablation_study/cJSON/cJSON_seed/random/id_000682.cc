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
//<ID> 682
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    char *printed = (char *)0;
    char json_buf[] = " { \"alpha\" : [ 10 , 20 ] , \"beta\" : \" spaced \" } ";
    int arr_size = 0;
    cJSON_bool added_meta = 0;
    cJSON_bool added_parsed = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    num0 = cJSON_CreateNumber(10.0);
    str0 = cJSON_CreateString("hello");
    meta = cJSON_CreateString("metadata");

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, str0);
    added_meta = cJSON_AddItemToObject(root, "meta", meta);
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    added_parsed = cJSON_AddItemToObject(root, "parsed", parsed);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)added_meta;
    (void)added_parsed;
    (void)arr_size;
    (void)printed;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}