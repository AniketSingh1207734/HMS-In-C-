#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>

class Prescription {
public:
    Prescription(std::string medication, int dosage, std::string frequency)
        : medication(medication), dosage(dosage), frequency(frequency) {}

    std::string toString() const {
        return "Medication: " + medication + ", Dosage: " + std::to_string(dosage) + "mg, Frequency: " + frequency;
    }

private:
    std::string medication;
    int dosage;
    std::string frequency;
};

class Patient {
public:
    Patient(std::string id, std::string name, int age, std::string gender, std::string diagnosis, std::string doctorAssigned)
        : id(id), name(name), age(age), gender(gender), diagnosis(diagnosis), doctorAssigned(doctorAssigned) {}

    void addPrescription(const Prescription& prescription) {
        prescriptions.push_back(prescription);
    }

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getGender() const { return gender; }
    std::string getDiagnosis() const { return diagnosis; }
    std::string getDoctorAssigned() const { return doctorAssigned; }
    std::vector<Prescription> getPrescriptions() const { return prescriptions; }

    std::string toString() const {
        std::string result = "Patient[ID=" + id + ", Name=" + name + ", Age=" + std::to_string(age) +
                             ", Gender=" + gender + ", Diagnosis=" + diagnosis + ", Doctor Assigned=" + doctorAssigned + "]\nPrescriptions:\n";
        for (const auto& prescription : prescriptions) {
            result += "  - " + prescription.toString() + "\n";
        }
        return result;
    }

private:
    std::string id;
    std::string name;
    int age;
    std::string gender;
    std::string diagnosis;
    std::string doctorAssigned;
    std::vector<Prescription> prescriptions;
};

class Doctor {
public:
    Doctor(std::string id, std::string name, std::string specialization)
        : id(id), name(name), specialization(specialization) {}

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getSpecialization() const { return specialization; }

    std::string toString() const {
        return "Doctor[ID=" + id + ", Name=" + name + ", Specialization=" + specialization + "]";
    }

private:
    std::string id;
    std::string name;
    std::string specialization;
};

class FileReaderUtil {
public:
    static std::vector<Patient> readPatients(const std::string& filename) {
        std::vector<Patient> patients;
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return patients;
        }

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string id, name, gender, diagnosis, doctorAssigned;
            int age;

            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            ss >> age;
            ss.ignore(1);
            std::getline(ss, gender, ',');
            std::getline(ss, diagnosis, ',');
            std::getline(ss, doctorAssigned, ',');

            patients.emplace_back(id, name, age, gender, diagnosis, doctorAssigned);
        }
        return patients;
    }

    static std::vector<Doctor> readDoctors(const std::string& filename) {
        std::vector<Doctor> doctors;
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return doctors;
        }

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string id, name, specialization;

            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, specialization, ',');

            doctors.emplace_back(id, name, specialization);
        }
        return doctors;
    }
};

class User {
    std::string username;
    std::string password;

public:
    // Default constructor (no arguments)
    User() : username(""), password("") {}

    // Parameterized constructor (with arguments)
    User(std::string username, std::string password)
        : username(username), password(password) {}

    // Getter for username
    std::string getUsername() const { return username; }

    // Function to validate password
    bool validatePassword(const std::string& pass) const { return password == pass; }
};


class HospitalManagementSystem {
public:
    void loadPatients(const std::string& filename) {
        patients = FileReaderUtil::readPatients(filename);
    }

    void loadDoctors(const std::string& filename) {
        doctors = FileReaderUtil::readDoctors(filename);
    }

    void loadUsers(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string username, password;

            std::getline(ss, username, ',');
            std::getline(ss, password, ',');

            users[username] = User(username, password);
        }
    }

    bool login(const std::string& username, const std::string& password) {
        if (users.find(username) != users.end() && users[username].validatePassword(password)) {
            currentUser = username;
            return true;
        }
        return false;
    }

    void displayPatients() const {
        std::cout << "Patients:\n";
        for (const auto& patient : patients) {
            std::cout << patient.toString() << std::endl;
        }
    }

    void displayDoctors() const {
        std::cout << "Doctors:\n";
        for (const auto& doctor : doctors) {
            std::cout << doctor.toString() << std::endl;
        }
    }

    void addPrescriptionToPatient(const std::string& patientId, const Prescription& prescription) {
        bool found = false;
        for (auto& patient : patients) {
            if (patient.getId() == patientId) {
                patient.addPrescription(prescription);
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Patient ID not found!\n";
        }
    }

private:
    std::vector<Patient> patients;
    std::vector<Doctor> doctors;
    std::unordered_map<std::string, User> users;
    std::string currentUser;
};

int main() {
    HospitalManagementSystem hms;
    hms.loadPatients("patients.txt");
    hms.loadDoctors("doctors.txt");
    hms.loadUsers("users.txt");

    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (hms.login(username, password)) {
        std::cout << "Login successful!\n";

        int choice;
        do {
            std::cout << "1. Display Patients\n";
            std::cout << "2. Display Doctors\n";
            std::cout << "3. Add Prescription to Patient\n";
            std::cout << "4. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    hms.displayPatients();
                    break;
                case 2:
                    hms.displayDoctors();
                    break;
                case 3:
                    {
                        std::string patientId, medication, frequency;
                        int dosage;

                        std::cout << "Enter Patient ID: ";
                        std::cin >> patientId;
                        std::cout << "Enter Medication: ";
                        std::cin >> medication;
                        std::cout << "Enter Dosage (mg): ";
                        while (!(std::cin >> dosage)) {
                            std::cout << "Invalid input. Please enter a valid dosage: ";
                            std::cin.clear();
                            std::cin.ignore(10000, '\n');
                        }
                        std::cout << "Enter Frequency: ";
                        std::cin >> frequency;
                        hms.addPrescriptionToPatient(patientId, Prescription(medication, dosage, frequency));
                    }
                    break;
                case 4:
                    std::cout << "Exiting...\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 4);
    } else {
        std::cout << "Invalid username or password!\n";
    }

    return 0;
}
