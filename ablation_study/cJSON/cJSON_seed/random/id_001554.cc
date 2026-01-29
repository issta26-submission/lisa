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
//<ID> 1554
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *note = (cJSON *)0;
    cJSON *retrieved_note = (cJSON *)0;
    cJSON_bool inserted_first = 0;
    cJSON_bool inserted_second = 0;
    cJSON_bool added_array_to_root = 0;
    cJSON_bool added_note_to_root = 0;
    char *note_str = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.25);
    num2 = cJSON_CreateNumber(3.5);
    note = cJSON_CreateString("ready");

    // step 3: Configure
    inserted_first = cJSON_InsertItemInArray(arr, 0, num1);
    inserted_second = cJSON_InsertItemInArray(arr, 1, num2);
    added_array_to_root = cJSON_AddItemToObject(root, "numbers", arr);
    added_note_to_root = cJSON_AddItemToObject(root, "status", note);

    // step 4: Operate
    retrieved_note = cJSON_GetObjectItem(root, "status");
    note_str = cJSON_GetStringValue(retrieved_note);

    // step 5: Validate
    validation_score = (int)inserted_first
        + (int)inserted_second
        + (int)added_array_to_root
        + (int)added_note_to_root
        + (int)(retrieved_note != (cJSON *)0)
        + (int)(note_str != (char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)note_str;
    (void)retrieved_note;
    (void)num1;
    (void)num2;
    (void)arr;
    (void)note;
    // API sequence test completed successfully
    return 66;
}