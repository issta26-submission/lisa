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
//<ID> 684
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    char *printed = (char *)0;
    char json_text[] = "  { \"demo\" : \"text\", \"x\" : 1 }  ";
    int arr_size = 0;
    cJSON_bool added_meta = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);
    meta = cJSON_CreateString("metadata");

    // step 3: Configure
    add_ok = cJSON_AddItemToArray(arr, n1);
    add_ok = cJSON_AddItemToArray(arr, n2);
    added_meta = cJSON_AddItemToObject(root, "meta", meta);

    // step 4: Operate
    cJSON_Minify(json_text);
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)added_meta;
    (void)add_ok;
    (void)arr_size;
    (void)printed;
    (void)json_text;
    (void)root;
    (void)arr;
    (void)n1;
    (void)n2;
    (void)meta;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}