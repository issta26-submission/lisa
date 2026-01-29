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
//<ID> 513
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_array = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *got_ints = (cJSON *)0;
    cJSON *got_str = (cJSON *)0;
    const char *greeting_text = (const char *)0;
    int numbers[4];
    int arr_size = 0;
    char buffer[64];
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    memset(numbers, 0, sizeof(numbers));
    memset(buffer, 0, sizeof(buffer));

    // step 2: Initialize
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    numbers[3] = 40;
    root = cJSON_CreateObject();
    int_array = cJSON_CreateIntArray(numbers, 4);
    str_item = cJSON_CreateString("hello world");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "ints", int_array);
    add_ok2 = cJSON_AddItemToObject(root, "greeting", str_item);

    // step 4: Operate
    got_ints = cJSON_GetObjectItem(root, "ints");
    arr_size = cJSON_GetArraySize(got_ints);
    got_str = cJSON_GetObjectItem(root, "greeting");
    greeting_text = (const char *)(got_str ? got_str->valuestring : (const char *)"");

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)buffer;
    (void)greeting_text;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}