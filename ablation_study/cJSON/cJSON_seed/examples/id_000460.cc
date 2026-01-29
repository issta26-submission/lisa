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
//<ID> 460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "person", person);
    cJSON *name1 = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name1);
    cJSON *age1 = cJSON_CreateNumber(30);
    cJSON_AddItemToObject(person, "age", age1);

    // step 2: Configure
    cJSON *address = cJSON_CreateObject();
    cJSON_AddItemToObject(person, "address", address);
    cJSON_AddStringToObject(address, "city", "Wonderland");

    // step 3: Operate & Validate
    cJSON *retrieved_person = cJSON_GetObjectItem(root, "person");
    cJSON *retr_name_item = cJSON_GetObjectItem(retrieved_person, "name");
    char *name_val = cJSON_GetStringValue(retr_name_item);
    cJSON *retr_age_item = cJSON_GetObjectItem(retrieved_person, "age");
    double age_val = cJSON_GetNumberValue(retr_age_item);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "before:name=%s|age=%.0f", name_val, age_val);

    cJSON *person2 = cJSON_CreateObject();
    cJSON_AddItemToObject(person2, "name", cJSON_CreateString("Bob"));
    cJSON_AddItemToObject(person2, "age", cJSON_CreateNumber(35));
    cJSON *addr2 = cJSON_CreateObject();
    cJSON_AddItemToObject(addr2, "city", cJSON_CreateString("Builderland"));
    cJSON_AddItemToObject(person2, "address", addr2);

    cJSON_ReplaceItemViaPointer(root, person, person2);

    cJSON *new_person = cJSON_GetObjectItem(root, "person");
    cJSON *new_name_item = cJSON_GetObjectItem(new_person, "name");
    char *new_name_val = cJSON_GetStringValue(new_name_item);
    cJSON *new_age_item = cJSON_GetObjectItem(new_person, "age");
    double new_age_val = cJSON_GetNumberValue(new_age_item);
    char final_buf[256];
    memset(final_buf, 0, sizeof(final_buf));
    sprintf(final_buf, "after:name=%s|age=%.0f", new_name_val, new_age_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}